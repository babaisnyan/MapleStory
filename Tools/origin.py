import unreal

ue_project_directory = '/Game/Mob/'
assetTools = unreal.AssetToolsHelpers.get_asset_tools()
asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
editor_asset_lib = unreal.EditorAssetLibrary()
datatable_func_lib = unreal.DataTableFunctionLibrary()
mob_folders = asset_registry.get_sub_paths(ue_project_directory, recurse=False)

asset_factory = unreal.DataTableFactory()
asset_factory.struct = unreal.load_object(None, '/Script/MapleStory.AnimationData')

for mob_folder in mob_folders:
    for action_folder in asset_registry.get_sub_paths(mob_folder, recurse=False):
        data_table = editor_asset_lib.load_asset(action_folder + '/DT_Frames.DT_Frames')
        assets = asset_registry.get_assets_by_path(action_folder, recursive=False)
        assets = sorted(assets, key=lambda asset: asset.get_full_name())
        offset_x = datatable_func_lib.get_data_table_column_as_string(data_table, 'OffsetX')
        offset_y = datatable_func_lib.get_data_table_column_as_string(data_table, 'OffsetY')
        index = 0
    
        for asset in assets:
            if not 'S_' in asset.get_full_name():
                continue
            
            sprite_asset = editor_asset_lib.load_asset(asset.get_export_text_name())
            x = int(offset_x[index])
            y = int(offset_y[index])
            index += 1
            
            sprite_asset.set_editor_property('pivot_mode', unreal.SpritePivotMode.CUSTOM)
            sprite_asset.set_editor_property('custom_pivot_point', unreal.Vector2D(x, y))
            
    unreal.EditorAssetLibrary.save_directory(mob_folder, recursive=True)
    unreal.SystemLibrary.collect_garbage()
    
unreal.EditorAssetLibrary.save_directory(ue_project_directory, recursive=True)
unreal.SystemLibrary.collect_garbage()