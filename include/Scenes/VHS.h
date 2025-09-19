#include "Scene.h"
#include <vector>

class Model;
class Shader;

class VHS : public Scene
{
    private:
    std::vector<Model*> models{};
    std::vector<Shader*> shaders{};

    public:
    VHS();
    ~VHS();
    void InitializeScene();
    // void DrawScene();
    // void MoveCamera(glm::vec3 direction);
    // void RotateCamera(float x, float y);
};