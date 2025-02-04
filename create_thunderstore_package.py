import subprocess, os, json, zipfile
from pathlib import Path
import build_n64recomp_tools as bnt
import build_mod as bm

package_dir = bm.project_root.joinpath("thunderstore_package")

deps = bnt.get_dependencies()
def get_git_url() -> str:
    result = subprocess.run(
        [
            deps["git"],
            "config", 
            "--get", 
            "remote.origin.url"
        ],
        cwd=bm.project_root,
        capture_output=True,
        text=True
    )

    if result.returncode == 0:
        print(f"Command output: {result.stdout}")
        return result.stdout.strip()
    else:
        print(f"Command failed with error: {result.stderr}")
        return None

def get_default_package_manifest() ->dict[str, str]:
    return {
        "name":  bm.mod_data["manifest"]["id"],
        "version_number":  bm.mod_data["manifest"]["version"],
        "website_url":  get_git_url(),
        "description":  bm.mod_data["manifest"]["short_description"],
        "dependencies":  []
    }


def create_package_directory():
    print(f"Creating package directory at '{package_dir}'...")
    os.makedirs(package_dir)

def create_manifest(path: Path):
    path.write_text(json.dumps(get_default_package_manifest(), indent=4))
    
def update_manifest(path: Path):
    current_manifest: dict[str, str] = json.loads(path.read_text());
    current_manifest.update({
        "name":  bm.mod_data["manifest"]["id"],
        "version_number":  bm.mod_data["manifest"]["version"],
        "description":  bm.mod_data["manifest"]["short_description"],
    })
    path.write_text(json.dumps(get_default_package_manifest(), indent=4))

def create_readme(path: Path):
    readme_str = f"# {bm.mod_data['manifest']['display_name']}\n\n{bm.mod_data['manifest']['description']}"
    path.write_text(readme_str)
    

def create_package():
    if not package_dir.is_dir():
        create_package_directory()
        
    manifest_file = package_dir.joinpath("manifest.json")
    if not manifest_file.is_file():
        create_manifest(manifest_file)
    else: 
        update_manifest(manifest_file)
        
    readme_file = package_dir.joinpath("README.md")
    if not readme_file.is_file():
        create_readme(readme_file)
    
    

if __name__ == '__main__':
    create_package()

