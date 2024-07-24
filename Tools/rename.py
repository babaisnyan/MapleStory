import unreal

ue_project_directory = '/Game/Item'
assetTools = unreal.AssetToolsHelpers.get_asset_tools()
asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()
assets = asset_registry.get_assets_by_path(ue_project_directory, recursive=True)

for asset in assets:
    old_name = asset.asset_name
    new_name = old_name

    new_name = new_name.replace('_Sprite', '')
    new_name = 'S_' + new_name[2:]   

    if old_name != new_name:
        package_path = asset.package_name
        package_path = package_path[:package_path.rfind('/') + 1]
        new_path = package_path + new_name

        try:
            renamed = assetTools.rename_asset(asset.get_editor_property('package_name'), new_path)
            if renamed:
                print(f"Renamed: {old_name} to {new_name}")
            else:
                print(f"Failed to rename: {old_name}")
        except Exception as e:
            print(f"Error renaming {old_name}: {str(e)}")

unreal.SystemLibrary.collect_garbage()
