import unreal

ue_project_directory = '/Game/Item'
assetTools = unreal.AssetToolsHelpers.get_asset_tools()
asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
assets = asset_registry.get_assets_by_path(ue_project_directory, recursive=False)

for asset in assets:
    old_name = asset.get_full_name()
    old_name = old_name.replace('/Script/Paper2D.PaperSprite ', '')
    new_name = old_name.replace('/Script/Paper2D.PaperSprite ', '')
    new_name = new_name.replace('_Sprite', '')
    new_name = new_name.replace('T_', 'S_')
    
    unreal.EditorAssetLibrary.rename_asset(old_name, new_name)

unreal.SystemLibrary.collect_garbage()
