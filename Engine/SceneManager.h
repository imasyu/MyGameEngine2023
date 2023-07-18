#pragma once
#include "GameObject.h"
class SceneManager :
    public GameObject
{
public:
    SceneManager(GameObject* parent);

    ~SceneManager();
    
    //‰Šú‰»
    void Initialize() override;

    //XV
    void Update() override;

    //•`‰æ
    void Draw() override;

    //‰ğ•ú
    void Release()override;
};

