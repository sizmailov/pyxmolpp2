import os
import json
import argparse
import subprocess


def git_revision():
    return subprocess.check_output(["git", "rev-parse", "HEAD"]).decode("utf-8").split("\n")[0]


def get_branch():
    return subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"]).decode("utf-8").split("\n")[0]


def source_tree_status():
    root_dir = subprocess.check_output(["git", "rev-parse", "--show-toplevel"]).decode('utf-8').strip()
    cmd = [
        'git', 'status',
        '-s', f'{root_dir}/src',
        '-s', f'{root_dir}/include',
        '-s', f'{root_dir}/tests/benchmarks',
    ]
    status = subprocess.check_output(cmd).decode('utf-8').strip()
    return status


if __name__ == "__main__":
    parser = argparse.ArgumentParser("run-benchmark")
    parser.add_argument("--benchmark", required=True)
    parser.add_argument("--name", required=True)

    args = parser.parse_args()

    revision = git_revision()[:8]

    name = args.name
    benchmark = args.benchmark
    branch = get_branch()

    tree_status = source_tree_status()
    if tree_status != "":
        print(f"git tree is not clean, abort benchmarks\n"
              f"{tree_status}")
        exit(1)

    if not os.path.exists(branch):
        os.mkdir(branch)

    if not os.path.exists(f"{branch}/{revision}"):
        os.mkdir(f"{branch}/{revision}")

    output = f"{branch}/{revision}/{name}.json"
    if os.path.exists(output):
        try:
            with open(output) as f:
                json.load(f)
            print(f"benchmark {output} already exist, skipped")
            exit(0)
        except Exception as e:
            print("Corrupted json: ", type(e), e)
            pass

    subprocess.call([
        benchmark,
        f"--benchmark_out={branch}/{revision}/{name}.json",
        f"--benchmark_out_format=json",
    ])
