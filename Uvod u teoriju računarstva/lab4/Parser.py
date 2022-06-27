import sys


def check_sequence(s:str) -> bool:
    global out, sequence
    out += s
    return not sequence


def C() -> bool:
    check_sequence('C')
    if A():
        return A()
    return False


def B() -> bool:
    global sequence
    if check_sequence('B') or sequence[0] != 'c':
        return True
    if len(sequence) >= 2 and sequence[1] == 'c':
        sequence = sequence[2:]
        if S() and len(sequence) >= 2 and sequence[0] == 'b' and sequence[1] == 'c':
            sequence = sequence[2:]
            return True
    return False


def A() -> bool:
    global sequence
    if check_sequence('A'):
        return False
    letter = sequence.pop(0)
    if letter == 'a':
        return True
    elif letter == 'b':
        return C()
    else:
        sequence.insert(0, letter)
        return False


def S() -> bool:
    global sequence
    if check_sequence('S'):
        return False
    letter = sequence.pop(0)
    if letter == 'a':
        if A():
            return B()
    elif letter == 'b':
        if B():
            return A()
    return False


if __name__ == '__main__':
    # f = open("src.txt", "r")
    # text = f.readline().strip()
    text = sys.stdin.readline().strip()
    sequence = list(text)
    out = ""
    if S() and not sequence:
        print(out)
        print("DA")
    else:
        print(out)
        print("NE")
