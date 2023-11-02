#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "resource.h"
#include "Engine/Camera.h"
#include <fstream>
#include <iostream>

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
    controlId = IDC_RADIO_UP;
}

void Stage::Update()
{
    if (!Input::IsMouseButtonDown(0)) {
        return;
    }
    else {
        // マウスの座標を取得
        XMFLOAT3 mousePos = Input::GetMousePosition();

        // マウスの座標を配列のインデックスに変換
        int clickedX = static_cast<int>(mousePos.x);
        int clickedZ = static_cast<int>(mousePos.z);

        // 配列の範囲内か確認
        if (clickedX >= 0 && clickedX < XSIZE && clickedZ >= 0 && clickedZ < ZSIZE) {
            // 指定した列の各行の高さを上げる
            int targetColumn = clickedX; // 上げたい列のインデックスを指定する（例: 0から14の間）
            for (int z = 0; z < ZSIZE; z++) {
                table_[targetColumn][z].height += 1;

                // 高さが15を超えないようにチェック（必要に応じて）
                if (table_[targetColumn][z].height > 15) {
                    table_[targetColumn][z].height = 15;
                }
            }
        }
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
    //レイを-5°移動させて、調節した
    float angleIncrement = XMConvertToRadians(-5.0f); // 角度をラジアンに変換
    XMMATRIX rotationMatrix = XMMatrixRotationX(angleIncrement); // X軸周りに回転
    invView = XMMatrixMultiply(rotationMatrix, invView); // ビュー行列に回転行列を適用
    XMFLOAT3 mousePosFront = Input::GetMousePosition();
    mousePosFront.z = 0.0f;
    XMFLOAT3 mousePosBack = Input::GetMousePosition();
    mousePosBack.z = 1.0f;
    //①　mousePosFrontをベクトルに変換
    XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront);
    //②　①にinvVP、invPrj、invViewをかける
    vMouseFront = XMVector2TransformCoord(vMouseFront, invVP * invProj * invView);
    //③　mousePosBackをベクトルに変換
    XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
    //④　③にinvVP、invPrj、invViewをかける
    vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

    rayHit_ = false;

    for (int x = 0; x < 15; x++)
    {
        for (int z = 0; z < 15; z++)
        {
            for (int y = 0; y < table_[x][z].height + 1; y++)
            {
                //⑤　②から④に向かってレイをうつ（とりあえずモデル番号はhModel_[0]）
                RayCastData data;
                XMStoreFloat4(&data.start, vMouseFront);
                XMStoreFloat4(&data.dir, vMouseBack - vMouseFront);
                Transform trans;
                trans.position_.x = x;
                trans.position_.y = y;
                trans.position_.z = z;
                Model::SetTransform(hModel_[0], trans);

                Model::RayCast(hModel_[0], data);

                //⑥　レイが当たったらブレークポイントで止める
                if (data.hit && !rayHit_) {
                    rayHit_ = true;
                    if (controlId == IDC_RADIO_UP) {
                        table_[x][z].height++;
                        break;
                    }
                    else if (controlId == IDC_RADIO_DOWN) {
                        if (y > 0) {
                            table_[x][z].height--;
                            break;
                        }
                    }
                    else if(controlId == IDC_COMBO1) {
                        SetBlock(x, z, (BLOCKTYPE)(comboId));
                        break;
                    }
                    else if (controlId == IDC_RADIO_MOVE_UP) {
                        // IDC_RADIO1が選択されたとき、指定した列の高さを1上げる処理
                        int targetColumn = 0; // 上げたい列のインデックスを指定する（例: 0から14の間）
                        if (targetColumn >= 0 && targetColumn < ZSIZE) {
                            for (int y = 0; y < XSIZE; y++) {
                                table_[y][z].height += 1;
                                // 高さが15を超えないようにチェック（必要に応じて）
                                if (table_[y][z].height > 15) {
                                    table_[y][z].height = 15;
                                }
                            }
                        }
                    }
                    else if (controlId == IDC_RADIO_MOVE_DOWN) {
                        int targetColumn = 0; // 下げたい列のインデックスを指定する
                        if (targetColumn >= 0 && targetColumn < ZSIZE) {
                            for (int y = 0; y < XSIZE; y++) {
                                table_[y][z].height -= 1;
                                // 高さが15を超えないようにチェック（必要に応じて）
                                if (table_[y][z].height > 15) {
                                    table_[y][z].height = 15;
                                }
                            }
                        }
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

string Stage::BlockData(const BLOCK& block)
{
    string data;
    data.resize(sizeof(BLOCK));
    memcpy(&data[0], &block, sizeof(BLOCK));
    return data;
}

void Stage::Save()
{
    char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数

    //「ファイルを保存」ダイアログの設定
    OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
    ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
    ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
    ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")        //─┬ファイルの種類
        TEXT("すべてのファイル(*.*)\0*.*\0\0");     //─┘
    ofn.lpstrFile = fileName;               	//ファイル名
    ofn.nMaxFile = MAX_PATH;               	//パスの最大文字数
    ofn.Flags = OFN_OVERWRITEPROMPT;   		//フラグ（同名ファイルが存在したら上書き確認）
    ofn.lpstrDefExt = "map";                  	//デフォルト拡張子

    //「ファイルを保存」ダイアログ
    BOOL selFile;
    selFile = GetSaveFileName(&ofn);

    //キャンセルしたら中断
    if (selFile == FALSE) return;

    //data.length()
    std::fstream outputFile(fileName, std::ios::binary | std::ios::out);

    for (int x = 0; x < XSIZE; x++) {
        for (int z = 0; z < ZSIZE; z++) {
            outputFile.write((char*)&table_[x][z], sizeof(BLOCK));
        }
    }
    outputFile.close();
}

void Stage::Load()
{
    char fileName[MAX_PATH] = "無題.map";  //ファイル名を入れる変数

    //「ファイルを保存」ダイアログの設定
    OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
    ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
    ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
    ofn.lpstrFilter = TEXT("マップデータ(*.map)\0*.map\0")        //─┬ファイルの種類
        TEXT("すべてのファイル(*.*)\0*.*\0\0");     //─┘
    ofn.lpstrFile = fileName;               	//ファイル名
    ofn.nMaxFile = MAX_PATH;               	//パスの最大文字数
    ofn.Flags = OFN_FILEMUSTEXIST;   		//フラグ（同名ファイルが存在したら上書き確認）
    ofn.lpstrDefExt = "map";                  	//デフォルト拡張子

    //「ファイルを開く」ダイアログ
    BOOL selFile;
    selFile = GetOpenFileName(&ofn);

    //キャンセルしたら中断
    if (selFile == FALSE) return;

    std::fstream inputFile(fileName, std::ios::binary | std::ios::in);

    for (int x = 0; x < XSIZE; x++) {
        for (int z = 0; z < ZSIZE; z++) {
            inputFile.read((char*)&table_[x][z], sizeof(BLOCK));
        }
    }
    inputFile.close();
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
        //コンボボックスの初期値
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("デフォルト"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("レンガ"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("草"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("砂"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, LPARAM("水"));
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_SETCURSEL, 0, 0);
        return TRUE;
    case WM_COMMAND:
        controlId = LOWORD(wp);
        comboId = SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0);
        
        return TRUE;
    }
    return FALSE;
}
