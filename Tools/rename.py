import unreal

ue_project_directory = '/Game/Mob'
assetTools = unreal.AssetToolsHelpers.get_asset_tools()
asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
assets = asset_registry.get_assets_by_path(ue_project_directory, recursive=True)

for asset in assets:
    if not asset.get_full_name().endswith('_Sprite'):
        continue
    
    file_name = asset.get_full_name().split('/')[-1]
    old_name = asset.get_full_name()
    old_name = old_name.replace('/Script/Paper2D.PaperSprite ', '')
    new_name = old_name.replace('/Script/Paper2D.PaperSprite ', '')
    new_name = new_name.replace('_Sprite', '')
    new_name = new_name.replace('T_', 'S_')
    new_name = new_name.replace('/Texture', '')
    unreal.EditorAssetLibrary.rename_asset(old_name, new_name)

unreal.EditorAssetLibrary.save_directory(ue_project_directory, recursive=True)
unreal.SystemLibrary.collect_garbage()
