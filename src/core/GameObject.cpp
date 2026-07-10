#include <core/GameObject.h>

// ===========================================================================
// GameObject：所有屏幕实体的基类，提供类似 Unity MonoBehaviour 的生命周期。
//   OnInit    —— 加入对象管理器时调用一次（Awake/Start）
//   OnUpdate  —— 每帧逻辑（Update）
//   OnRender  —— 每帧绘制（可选）
//   OnDestroy —— 移除时调用一次
// 子类必须实现 GetType() 用于类型查询。
// 碰撞采用圆形判定，用 radius 表示判定半径。
// ===========================================================================

struct GameContext;

class GameObject {
public:
    GameObject();
    virtual ~GameObject() = default;

    // —— 生命周期 ——
    virtual void OnInit() {}
    virtual void OnUpdate(float dt) {}
    virtual void OnRender(sf::RenderTarget& target) {}
    virtual void OnDestroy() {}

    // —— 类型 ——
    virtual ObjectType GetType() const = 0;

    // —— 上下文 ——
    void          SetContext(GameContext* ctx);
    GameContext*  GetContext() const { return context; }

    // —— 变换 ——
    const sf::Vector2f& GetPosition() const { return position; }
    void          SetPosition(const sf::Vector2f& pos) { position = pos; }
    void          Move(const sf::Vector2f& delta) { position += delta; }

    float         GetRotation() const { return rotation; }
    void          SetRotation(float degrees) { rotation = degrees; }

    // —— 激活 / 销毁 ——
    bool          IsActive() const { return active; }
    void          SetActive(bool a) { active = a; }

    bool          IsDead() const { return dead; }
    void          Destroy(); // 标记待移除

    // —— 碰撞 ——
    float         GetRadius() const { return radius; }
    void          SetRadius(float r) { radius = r; }

protected:
    GameContext*    context = nullptr;
    sf::Vector2f    position;
    float           rotation = 0.0f;
    float           radius = 16.0f;   // 圆形判定半径
    bool            active = true;
    bool            dead = false;
    sf::Sprite      sprite;           // 子类可自行使用
};
