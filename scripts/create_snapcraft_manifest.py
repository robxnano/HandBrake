#!/usr/bin/env python3

import types
import os
import sys
import yaml
import getopt
import posixpath
from collections import OrderedDict
from urllib.parse import urlsplit, unquote


def url2filename(url):
    path = urlsplit(url).path
    return posixpath.basename(unquote(path))

def islocal(url):
    result = urlsplit(url)
    return result.scheme == "" and result.netloc == ""

class SourceType:
    contrib = 1
    archive = 2

class SourceEntry:
    def __init__(self, url, entry_type, sha256=None):
        self.url        = url
        self.entry_type = entry_type
        self.sha256     = sha256

class SnapcraftPluginManifest:
    def __init__(self, runtime, template=None):
        if template is not None and os.path.exists(template):
            with open(template, 'r') as fp:
                self.manifest = yaml.safe_load(fp)

        else:
            self.manifest    = OrderedDict()

class SnapcraftManifest:
    def __init__(self, source_archive, features, template=None):
        if template is not None and os.path.exists(template):
            with open(template, 'r') as fp:
                self.manifest = yaml.safe_load(fp)

            self.parts       = self.manifest["parts"]
            self.handbrake   = self.parts["handbrake"]
            self.build_env   = self.handbrake["build-environment"]
            self.build_flags = ['--snap', '--prefix=/usr', '--build=build-snap']
        else:
            print(f"{template} not found")
            exit(1)

        if "nvenc" in features:
            self.build_flags += ['--enable-nvenc', '--enable-nvdec']

        if "vce" in features:
            self.build_flags += ['--enable-vce']

        if "fdk-aac" in features:
            self.build_flags += ['--enable-fdk-aac']

        if "qsv" in features:
            self.build_flags += ['--enable-qsv']

        for i in self.build_env:
            if "BUILD_FLAGS" in i:
                i["BUILD_FLAGS"] = ' '.join(self.build_flags)

        if "libdovi" not in features:
            self.parts.pop("rust-toolchain")
            self.handbrake.pop("after")
            self.handbrake["build-packages"].remove("rustup")

        if source_archive:
            self.handbrake["source-type"] = "tar"
            self.handbrake["source"] = source_archive

def usage():
    print("create_snapcraft_manifest [-a <archive>] [-t <template>] [-f <feature>] [-p] [<dst>]")
    print("     -a --archive    - Main archive (a.k.a. HB sources)")
    print("     -t --template   - snapcraft.yaml template")
    print("     -f --feature    - Build with <feature> support")
    print("     -p --plugin     - Manifest if for a HandBrake snap plugin")
    print("     -h --help       - Show this message")

if __name__ == "__main__":
    try:
        opts, args = getopt.getopt(sys.argv[1:], "a:t:f:ph",
            ["archive=", "template=", "feature=", "plugin", "help"])
    except getopt.GetoptError:
        print("Error: Invalid option")
        usage()
        sys.exit(2)

    if len(args) > 1:
        usage()
        exit(2)

    source_archive = None
    template = "snapcraft.yaml"
    plugin = 0
    features = []
    # exit()
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
            sys.exit()
        elif opt in ("-a", "--archive"):
            source_archive = arg
        elif opt in ("-t", "--template"):
            template = arg
        elif opt in ("-f", "--feature"):
            features.append(arg)
        elif opt in ("-p", "--plugin"):
            plugin = 1

    if len(args) > 0:
        dst = args[0]
    else:
        dst = None

    if plugin:
        manifest = SnapcraftPluginManifest(template)
    else:
        manifest = SnapcraftManifest(source_archive, features, template)

    if dst is not None:
        with open(dst, 'w') as fp:
            yaml.safe_dump(manifest.manifest, fp, sort_keys=False)
    else:
        print(yaml.safe_dump(manifest.manifest, sort_keys=False))

