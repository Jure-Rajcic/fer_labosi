import sys

global Q, Σ, δ, qo, F   # dak = (Q, Σ, δ, qo, F)
global xxx, table


def init_table():
    table = [[0 for i in range(len(Σ) + 1)] for j in range(len(Q))]
    for i in range(4, len(text)):
        line = text[i].split("->")
        q = line[0].split(",")[0]
        if Q.__contains__(q):
            symbol = line[0].split(",")[1]
            δ = line[1]
            table[Q.index(q)][Σ.index(symbol)] = δ
            if F.__contains__(q):
                table[Q.index(q)][len(Σ)] = 1
    return table


def get_possible_states(q, visited_states):
    if visited_states[Q.index(q)] == 1:
        return []
    return_list = [q]
    visited_states[Q.index(q)] = 1
    for symbol in Σ:
        p = table[Q.index(q)][Σ.index(symbol)]
        return_list.extend(get_possible_states(p, visited_states))
    return return_list


def fill_list_recursively(p, q):
    if xxx[Q.index(q)][Q.index(p)] == 1:
        return
    # print(p)
    # print(q)
    for previous in xxx[Q.index(q)][Q.index(p)]:
        fill_list_recursively(previous[0], previous[1])
    xxx[Q.index(q)][Q.index(p)] = 1


def output_format(x):
    return str(x).replace("[", "").replace("', '", ",").replace("]", "").replace("'", "")


if __name__ == '__main__':
    # |------------------------------------data------------------------------------|
    f = open("src.txt", "r")
    text = f.read().splitlines()
    # text = sys.stdin.read().splitlines()
    Q = sorted(text[0].split(","))
    Σ = sorted(text[1].split(","))
    F = sorted(text[2].split(","))
    q0 = text[3]
    table = init_table()
    Q = sorted(get_possible_states(q0, [0 for i in range(len(Q))]))
    table = init_table()
    # | ------------------------------------algorithm3 ------------------------------------ |
    xxx = [[[] for i in range(len(Q))] for j in range(len(Q))]
    for i in range(len(Q)):
        for j in range(i + 1, len(Q)):
            p = Q[i]
            q = Q[j]
            if F.__contains__(p) and not F.__contains__(q) or not F.__contains__(p) and F.__contains__(q):
                xxx[Q.index(q)][Q.index(p)] = 1

    for i in range(0, len(Q)):
        for j in range(i + 1, len(Q)):
            for k in range(0, len(Σ)):
                pair_pq = sorted([Q[i], Q[j]])
                p = pair_pq[0]
                q = pair_pq[1]
                δ_pair_pq = sorted([table[i][k], table[j][k]])
                δp = δ_pair_pq[0]
                δq = δ_pair_pq[1]
                if xxx[Q.index(δq)][Q.index(δp)] == 1:
                    fill_list_recursively(p, q)
                    xxx[Q.index(q)][Q.index(p)] = 1
                    break
                else:
                    if δp != δq:
                        xxx[Q.index(δq)][Q.index(δp)].append(pair_pq)
    # | ------------------------------------removing identical states------------------------------------- |
    identical_states = [[Q[j], Q[i]] for i in range(1, len(Q)) for j in range(0, i) if xxx[i][j] != 1]
    identical_states = sorted(identical_states, reverse=True)
    arr = [0 for i in range(len(Q))]
    for pairs in identical_states:
        p = pairs[0]
        q = pairs[1]
        arr[Q.index(q)] = p
        for j in range(Q.index(q) + 1, len(Q)):
            if arr[j] == q:
                arr[j] = p
    arr = [Q[i] if arr[i] == 0 else arr[i] for i in range(len(arr))]
    Q1 = [Q[i] for i in range(len(arr)) if arr[i] == Q[i]]
    # | ------------------------------------printing output------------------------------------- |
    Qn = [value for value in Q1 if value in Q]
    print(output_format(Qn))
    print(output_format(Σ))
    Fn = [value for value in F if value in Qn]
    print(output_format(Fn))
    print(arr[Q.index(q0)])
    for q in Qn:
        for symbol in Σ:
            candidate = table[Q.index(q)][Σ.index(symbol)]
            print(f"{q},{symbol}->{arr[Q.index(candidate)]}")
