#include "MainConverter.h"

int main()
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

    std::wstring filePath{ TEXT("") };
    _uint convertType{ 0 };
    std::wcout << TEXT("Input Convert Type") << std::endl;

    std::wcout << TEXT("0. NonAnim | 1. Anim") << std::endl;

    std::wcin >> convertType;

    std::wcout << TEXT("Input File Path : ");
    std::wcin >> filePath;

    Assimp::CMainConverter::Convert(convertType, filePath);

    return 0;
}
