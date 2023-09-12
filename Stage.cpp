#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "resource.h"
#include "Engine/Camera.h"

Stage::Stage(GameObject* parent)
    :GameObject(parent, "Stage")
{
    for (int i = 0; i < MODEL_NUM; i++)
    {
        hModel_[i] = -1;
    }
    //tableの初期化
    for (int x = 0; x < MODEL_NUM; x++) {
        for (int z = 0; z < MODEL_NUM; z++) {
            //table_[x][z] = 0;
            SetBlock(x, z, DEFAULT);
        }
    }
}

void Stage::Initialize()
{
    string modelname[] = {
        "BoxDefault.fbx",
        "BoxBrick.fbx",
        "BoxGrass.fbx",
        "BoxSand.fbx",
        "BoxWater.fbx"
    };
    string fname_base = "Assets/";
    //モデルデータのロード
    for (int i = 0; i < MODEL_NUM; i++)
    {
        hModel_[i] = Model::Load(fname_base + modelname[i]);
        assert(hModel_[i] >= 0);
    }

    //tableにブロックのタイプをセット
    for (int z = 0; z < ZSIZE; z++) {
        for (int x = 0; x < XSIZE; x++) {
            //table_[x][z] = x % 5;
            //SetBlock(x, z, (BLOCKTYPE)(z%5));
            //SetBlockHeight(x, z, x%4);
            SetBlock(x, z, (BLOCKTYPE)(0));
            SetBlockHeight(x, z, 0);
        }
    }
}

void Stage::Update()
{
    if (!Input::IsMouseButtonDown(0)) {
        return;
    }
    float w = (float)(Direct3D::scrWidth / 2.0f);
    float h = (float)(Direct3D::scrHeight / 2.0f);
    //0ffsetx.yは0
    //minZ = 0 maxZ = 1

    XMMATRIX vp =
    {
        w,  0, 0, 0,
        0, -h, 0, 0,
        0,  0, 1, 0,
        w,  h, 0, 1
    };

    //ビューポート
    XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
    //プロジェクション変換
    XMMATRIX invProj = XMMatrixInverse(nullptr, Camera::GetProjectionMatrix());
    //ビュー変換
    XMMATRIX invView = XMMatrixInverse(nullptr, Camera::GetViewMatrix());
    XMFLOAT3 mousePosFront = Input::GetMousePosition();
    mousePosFront.z = 0.0f;
    XMFLOAT3 mousePosBack = Input::GetMousePosition();
    mousePosBack.z = 1.0f;
    //�@　mousePosFrontをベクトルに変換
    XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);
    //�A　�@にinvVP、invPrj、invViewをかける
    vMouseFront = XMVector2TransformCoord(vMouseFront, invVP * invProj * invView);
    //�B　mousePosBackをベクトルに変換
    XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
    //�C　�BにinvVP、invPrj、invViewをかける
    vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            for (int y = 0; y < table_[x][z].height + 1; y++)
            {
                //�D　�Aから�Cに向かってレイをうつ（とりあえずモデル番号はhModel_[0]）
                RayCastData data;
                XMStoreFloat4(&data.start, vMouseFront);
                XMStoreFloat4(&data.dir, vMouseBack - vMouseFront);
                Transform trans;
                trans.position_.x = x;
                trans.position_.y = y;
                trans.position_.z = z;
                Model::SetTransform(hModel_[0], trans);

                Model::RayCast(hModel_[0], data);

                //�E　レイが当たったらブレークポイントで止める
                if (data.hit)
                {
                    table_[x][z].height++;
                    break;
                }
                else if (controlId == IDC_RADIO_DOWN) {
                    if (data.hit) {
                        table_[x][z].height--;
                        break;
                    }
                }
                
            }
        }
    }
}

void Stage::Draw()
{
    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            for (int y = 0; y < table_[x][z].height + 1; y++)
            {
                //table[x][z]からオブジェクトのタイプを取り出して書く
                int type = table_[x][z].type;
                Transform trans;
                trans.position_.x = x;
                trans.position_.y = y;
                trans.position_.z = z;
                Model::SetTransform(hModel_[type], trans);
                Model::Draw(hModel_[type]);
            }
        }
    }
}

void Stage::Release()
{
}

void Stage::SetBlock(int _x, int _z, BLOCKTYPE _type)
{
    //エラーチェック　範囲内の値かどうか　したほういい
    table_[_x][_z].type = _type;
}

void Stage::SetBlockHeight(int _x, int _z, int _height)
{
    //エラーチェック　範囲内の値かどうか　したほういい
    table_[_x][_z].height = _height;
}

//もう一つのプロシージャ
BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    //ダイアログできた
    case WM_INITDIALOG:
        //ラジオボタンの初期値
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_DOWN), BM_SETCHECK, BST_CHECKED, 0);

        //コンボボックスの初期値
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("デフォルト"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("レンガ"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("ガラス"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("砂"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("水"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_SETCURSEL, 0, 0);
    case WM_COMMAND:
        controlId = LOWORD(wp);
        return TRUE;
    }
    return FALSE;
}