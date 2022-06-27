import os
import time

if __name__ == '__main__':
    ok = "OK: "
    not_ok ="NOT OK: "
    for i in range(1, 53 + 1): # 1, 33
        os.system('cmd /c "color c"')
        s = f'testcases\\test{"%02d" % (i)}'

        print(f'processing test {i}', end=" -> ")
        t1 = time.time()
        os.system(f'cmd /c python SimEnka.py < {s}\\test.a > {s}\\result.txt')
        print(f"[{round(time.time() - t1)}]", end=" seconds\n")
        their_result = open(f'{s}\\test.b')
        my_result = open(f'{s}\\result.txt')
        if my_result.read() == their_result.read():
            ok += f'{i} '
        else:
            not_ok += f'{i} '
    print(ok)
    print(not_ok)
