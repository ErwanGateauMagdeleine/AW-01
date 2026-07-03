import pytest
from conftest import windows_only, mac_only
import subprocess


@windows_only
def test_vst3_is_flat_dll(install_vst3):
    """
    Tests that the plugin can be installed properly.
    """
    assert install_vst3.exists(), f"Expected VST3 bundle at {install_vst3}"
    assert not install_vst3.is_dir(), f"Bundle is the VST3 file and not the dir {install_vst3}"


@mac_only
def test_vst3_bundle_exists(install_vst3):
    bundle = install_vst3
    assert bundle.is_dir(), f"VST3 bundle missing at {bundle}"


@mac_only
def test_vst3_info_plist_exists(install_vst3):
    bundle = install_vst3
    plist = bundle / "Contents" / "Info.plist"
    assert plist.is_file(), f"Info.plist missing — bundle is incomplete: {plist}"


@mac_only
def test_vst3_binary_exists(install_vst3):
    bundle = install_vst3
    binaries = list((bundle / "Contents" / "MacOS").glob("*"))
    assert len(binaries) == 1 and binaries[0].is_file(), \
        f"Expected exactly one binary in Contents/MacOS/, found: {binaries}"

def test_plugin_passes_pluginval(install_vst3, pluginval_binary):
    bundle = install_vst3
    pluginval_bin = pluginval_binary

    res = subprocess.run(
        [
            str(pluginval_bin),
            "--strictness-level", "5",
            "--skip-gui-tests",
            "--validate-in-process",
            "--verbose",
            str(bundle)
        ],
        capture_output=True,
        text = True)

    print(f"stdout::\n{res.stdout}\n")
    assert res.returncode == 0, \
        (f"pluginval failed with strictness level 5\n"
         f"stderr:\n{res.stderr}")
