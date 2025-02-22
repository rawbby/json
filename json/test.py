import glob
import sys
from os.path import dirname, abspath, join
from subprocess import run, PIPE

source_dir = join(abspath(dirname(__file__)), '..')
test_dir = join(abspath(dirname(__file__)), 'test')
build_dir = join(source_dir, 'cmake-build-release')
build_type = 'release'

json_exe = join(build_dir, 'bin', 'json')
test_status = 0


def test(code, *files):
    global test_status

    for file in files:
        print(file)

    stdout = run([json_exe, *files], cwd=test_dir, check=True, stdout=PIPE, stderr=PIPE, text=True).stdout.strip()
    expect = f"{code}" * len(stdout)

    print('expect lines equal')
    print(expect)
    print(stdout)
    print()

    if expect != stdout:
        test_status = 1


if __name__ == '__main__':
    run(['python', 'generator.py']).check_returncode()
    run(['cmake', '-S', '.', '-B', build_dir, f'-DCMAKE_BUILD_TYPE={build_type}', '-DCLANG_FORMAT_TARGET=ON'],
        cwd=source_dir).check_returncode()
    run(['cmake', '--build', build_dir, '--target', 'clang_format'], cwd=source_dir).check_returncode()
    run(['cmake', '--build', build_dir, '--target', 'json'], cwd=source_dir).check_returncode()

    test(0, *glob.glob(join(test_dir, '0', '*.json')))
    test(1, *glob.glob(join(test_dir, '1', '*.json')))

    sys.exit(test_status)
