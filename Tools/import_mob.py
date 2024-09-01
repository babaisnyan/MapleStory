import unreal
import os

source_directory = "O:\\Unreal Projects\\MapleStory\\Tools\\MobExporter\\bin\\Release\\net8.0\\Exported"
ue_project_directory = "/Game/Mob"
assetTools = unreal.AssetToolsHelpers.get_asset_tools()
asset_factory = unreal.DataTableFactory()
asset_factory.struct = unreal.load_object(None, "/Script/MapleStory.AnimationData")
import_png = False
imported_count = 0

for subdir, dirs, files in os.walk(source_directory):
    for dir in dirs:
        mob_path = ue_project_directory + "/" + dir

        if dir.isdigit():
            unreal.EditorAssetLibrary.make_directory(mob_path)

        for action_dir, action_name, _ in os.walk(os.path.join(subdir, dir)):
            for action in action_name:
                action_path = mob_path + "/" + action
                source_file_path = os.path.join(subdir, dir, action)
                ue_texture_directory = mob_path + "/" + action + "/Texture/"

                assetImportData = unreal.AutomatedAssetImportData()
                assetImportData.destination_path = ue_texture_directory
                assetImportData.replace_existing = True
                assetImportData.factory = unreal.TextureFactory()
                assetImportData.filenames = []

                unreal.EditorAssetLibrary.make_directory(action_path)

                for file in os.listdir(os.path.join(subdir, dir, action)):
                    if file.endswith(".png") and import_png:
                        assetImportData.filenames.append(
                            os.path.join(source_file_path, file)
                        )
                    elif file.endswith(".csv") and not import_png:
                        ue_data_directory = mob_path + "/" + action
                        assetImportData = unreal.AssetImportTask()
                        assetImportData.filename = os.path.join(source_file_path, file)
                        assetImportData.destination_path = ue_data_directory
                        assetImportData.replace_existing = True
                        assetImportData.automated = True
                        assetImportData.destination_name = f'{file.split(".")[0]}'
                        csv_factory = unreal.CSVImportFactory()
                        csv_factory.automated_import_settings.import_row_struct = (
                            asset_factory.struct
                        )
                        assetImportData.factory = csv_factory

                        assetTools.import_asset_tasks([assetImportData])
                        imported_count += 1

                if import_png:
                    assetTools.import_assets_automated(assetImportData)
                    imported_count += 1

        if imported_count % 100 == 0:
            unreal.EditorAssetLibrary.save_directory(mob_path)
            unreal.SystemLibrary.collect_garbage()

unreal.EditorAssetLibrary.save_directory(ue_project_directory)
unreal.SystemLibrary.collect_garbage()
print("Import Finished")
