# U skupu {0, 1, 2, . . . , v − 1} nadjite k-člani podskup {a1, a2, . . . , ak } tako
# da se u skupu svih k(k − 1) razlika oblika ai − aj (mod v) svaki od brojeva
# {1, 2, . . . , v − 1} pojavi točno λ puta.

import time

(v, k, λ) = (11, 5, 2)  # k(k-1) = λ(v-1) ili (11, 5, 2)
num = 0


def check_differences(curr_list, last_check=False) -> bool:
    differences = [0] * (v - 1)
    for el1 in curr_list:
        for el2 in curr_list:
            index = el1 - el2 + (v if el1 < el2 else 0)
            # modulo = 11 if el1 < el2 else 0
            # print(el1, '-', el2, '+', modulo, '=', el1 - el2 + modulo)
            if index == 0:
                continue
            differences[index - 1] += 1
            if differences[index - 1] > λ:
                return False

    if last_check:
        # print(curr_list, differences)
        for λ_i in differences:
            if λ_i != λ:
                return False
    return True


def f(i, k_left, curr_list):
    # print(curr_list)

    # if i == v or not check_differences(curr_list):
    if i == v + 1 or not check_differences(curr_list):
        return
    if k_left == 0:
        if check_differences(curr_list, last_check=True):
            global num
            num += 1
            print(f"{num}. {curr_list}")
        return

    while i < v - k_left + 1:
        curr_list.append(i)
        f(i + 1, k_left - 1, curr_list)
        curr_list.pop()
        i += 1


if __name__ == '__main__':
    start_time = time.time()
    if k * (k-1) != λ * (v-1):
        print("Rjesenje nemoze postojati")
    else:
        f(0, k, [])
    print("--- %s seconds ---" % (time.time() - start_time))
    # print(check_differences([0,1,3,6,10]))

