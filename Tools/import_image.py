import unreal
import os
import gc  

source_directory = 'O:\\Unreal Projects\\MapleStory\\Tools\\ItemExporter\\bin\\Release\\net8.0\\Exported'
ue_project_directory = '/Game/Item/Texture'
assetTools = unreal.AssetToolsHelpers.get_asset_tools()

# 배치 크기 설정
batch_size = 100
files_processed = 0

for subdir, dirs, files in os.walk(source_directory):
    for file in files:
        if file.endswith('.png'):
            source_file_path = os.path.join(subdir, file)
            relative_path = os.path.relpath(subdir, source_directory)
            ue_texture_directory = ue_project_directory + '/' + relative_path.replace('\\', '/')
  
            assetImportData = unreal.AutomatedAssetImportData()
            assetImportData.destination_path = ue_texture_directory
            assetImportData.filenames = [source_file_path]
            assetImportData.factory = unreal.TextureFactory()
            assetImportData.replace_existing = True
            
            result = assetTools.import_assets_automated(assetImportData)
            imported_path = result[0].get_full_name()
            print('Imported: ' + file)

            files_processed += 1
            if files_processed % batch_size == 0:
                unreal.EditorAssetLibrary.save_directory(ue_texture_directory, recursive=True)
                unreal.SystemLibrary.collect_garbage()  # 가비지 컬렉션 수행
                print(f'Processed {files_processed} files, performed garbage collection.')

# 남은 파일이 있으면 가비지 컬렉션 수행
if files_processed % batch_size != 0:
    unreal.SystemLibrary.collect_garbage()
    print('Final garbage collection performed.')
