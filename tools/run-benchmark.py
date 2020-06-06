import os
import json
import argparse
import subprocess


def git_revision():
    return subprocess.check_output(["git", "rev-parse", "HEAD"]).decode("utf-8").split("\n")[0]


def get_branch():
    return subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"]).decode("utf-8").split("\n")[0]


if __name__ == "__main__":
    parser = argparse.ArgumentParser("run-benchmark")
    parser.add_argument("--benchmark", required=True)
    parser.add_argument("--name", required=True)

    args = parser.parse_args()

    revision = git_revision()[:8]

    name = args.name
    benchmark = args.benchmark
    branch = get_branch()

    if not os.path.exists(branch):
        os.mkdir(branch)

    if not os.path.exists(f"{branch}/{revision}"):
        os.mkdir(f"{branch}/{revision}")

    subprocess.call([
        benchmark,
        f"--benchmark_out={branch}/{revision}/{name}.json",
        f"--benchmark_out_format=json",
    ])
