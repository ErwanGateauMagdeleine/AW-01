import pytest
import platform
from pathlib import Path
import subprocess
import os
import stat
import shutil


IS_WINDOWS = platform.system() == "Windows"
IS_MAC = platform.system() == "Darwin"


mac_only = pytest.mark.skipif(not IS_MAC, reason="macOS bundle structure only applies on macOS")
windows_only = pytest.mark.skipif(not IS_WINDOWS, reason="Windows-only test")


def _force_rmtree(path):
    """shutil.rmtree, but handles NSIS-installed files marked read-only."""
    def remove_readonly(func, path, exc_info):
        os.chmod(path, stat.S_IWRITE)
        func(path)
    shutil.rmtree(path, onerror=remove_readonly)


@pytest.fixture
def get_plugin_installer_path():
    """
    Returns the path of the plugin installer. If the platform running the test
    is not neither windows or MacOs or if the path doesn't exist, it will mark the
    test as failed.

    Returns
    -------
    path (str): The path of the executable.
    """
    if IS_WINDOWS:
        path = Path("build/AW-01.exe").resolve()
    elif IS_MAC:
        path = Path("build/AW-01.pkg").resolve()
    else:
        pytest.fail("Unsupported platform for installer testing")

    if not path.exists():
        pytest.fail("Installer Path does not exist")

    return path


@pytest.fixture
def install_vst3(get_plugin_installer_path, tmp_path_factory):
    if IS_WINDOWS:
        install_dir = tmp_path_factory.mktemp("aw01_install", numbered=False)
        result = subprocess.run([str(get_plugin_installer_path), "/S", f"/D={install_dir}"], capture_output=True, text=True)
        assert result.returncode == 0, f"Silent install failed:\n{result.stderr}"
        vst3 = install_dir / "AW-01.vst3"

    else:  # macOS — no temp-path override exists, installs to the real system path
        result = subprocess.run(["sudo", "installer", "-pkg", str(get_plugin_installer_path), "-target", "/"], capture_output=True, text=True)
        assert result.returncode == 0, f"Silent install failed:\n{result.stderr}"
        vst3 = Path("/Library/Audio/Plug-Ins/VST3/AW-01.vst3")

    yield vst3

    if IS_WINDOWS:
        _force_rmtree(install_dir)
    if IS_MAC and vst3.exists():
        subprocess.run(["sudo", "rm", "-rf", str(install_dir)])
