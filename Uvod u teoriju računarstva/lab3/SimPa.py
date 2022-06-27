import sys

# Q, Σ, Γ, F, q0, Z0, δ
# a, b, c, d, e, f, g

if __name__ == '__main__':
    f = open("src.txt", "r")
    text = f.read().splitlines()
    # text = sys.stdin.read().splitlines()
    strings = text[0].split("|")
    Q = text[1].split(",")
    Σ = text[2].split(",")
    Γ = text[3].split(",")
    F = text[4].split(",")
    q0 = text[5]
    Z0 = text[6]
    δ = {}
    for i in range(7, len(text)):
        x = text[i].split("->")
        δ[x[0]] = x[1]
    for string in strings:
        q_curr = q0
        stack = Z0
        output = f"{q_curr}#{stack}|"
        flag = False
        for y in string.split(","):
            # print(q_curr)
            # print(output)
            # print(stack)
            key = f"{q_curr},{y},{stack[0]}"
            # print(key)
            if key not in δ.keys():
                x_key = f"{q_curr},$,{stack[0]}"
                while x_key in δ.keys() and key not in δ.keys():
                    value = δ[x_key].split(",")
                    q_curr = value[0]
                    stack = value[1].replace("$", "") + stack[1:]
                    if stack == "":
                        stack = "$"
                    output += f"{q_curr}#{stack}|"
                    x_key = f"{q_curr},$,{stack[0]}"
                    key = f"{q_curr},{y},{stack[0]}"
                if x_key not in δ.keys() and key not in δ.keys():
                    output += 'fail|'
                    flag = True
                    break
            # print(key)
            value = δ[key].split(",")
            q_curr = value[0]
            value[1] = value[1].replace("$", "")
            stack = value[1] + stack[1:]
            output += f"{q_curr}#{stack}|"

        if flag:
            output += "0"
        else:
            x_key = f"{q_curr},$,{stack[0]}"
            while x_key in δ.keys() and q_curr not in F:
                value = δ[x_key].split(",")
                q_curr = value[0]
                stack = value[1].replace("$", "") + stack[1:]
                if stack == "":
                    stack = "$"
                output += f"{q_curr}#{stack}|"
                x_key = f"{q_curr},$,{stack[0]}"
            if q_curr in F:
                output += "1"
            else:
                output += "0"
        print(output)
