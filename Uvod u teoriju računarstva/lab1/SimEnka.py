import sys

states = None
alphabet = None
table = None
visited_states = None


def e_environment(string_state):
    return_set = set()
    if string_state != "#" and visited_states[states.index(string_state)] == 0:
        return_set.add(string_state)
        visited_states[states.index(string_state)] = 1
        next_state_set = table[states.index(string_state)][alphabet.index("$")]
        for state in next_state_set:
            return_set = set.union(return_set, e_environment(state))
    return return_set


if __name__ == '__main__':
    # getting data
    # f = open("src.txt", "r")
    # text = f.read().splitlines()
    text = sys.stdin.read().splitlines()
    sequences = text[0].split("|")
    states = text[1].split(",")
    alphabet = text[2].split(",")
    alphabet.append("$")
    acceptable_states = text[3].split(",")
    starting_state = text[4]
    rows, cols = (len(states), len(alphabet))
    table = [[set("#") for i in range(cols)] for j in range(rows)]
    cnt_rows = 0
    cnt_cols = 0
    for i in range(5, len(text)):
        line = text[i].split("->")
        current_state = line[0].split(",")[0]
        current_sequence = line[0].split(",")[1]
        next_states = set(line[1].split(","))
        table[states.index(current_state)][alphabet.index(current_sequence)] = next_states
    # forming logic
    for sequence in sequences:
        visited_states = [0] * rows
        curr_set = e_environment(starting_state)
        output_string = str(sorted(curr_set)) + '|'

        for word in sequence.split(","):
            visited_states = [0] * rows
            output_set = set()
            for next_state_string in curr_set:
                if next_state_string == "#":
                    continue
                next_state_set = table[states.index(next_state_string)][alphabet.index(word)]
                output_set = set.union(output_set, next_state_set)
            for string_state in output_set:
                output_set = set.union(output_set, e_environment(string_state))
            curr_set = output_set
            if len(output_set) == 0:
                output_set.add("#")
            if output_set.__len__() != 1 and output_set.__contains__("#"):
                output_set.remove("#")
            output_string += str(sorted(output_set)) + '|'
            for s in "' []":
                output_string = output_string.replace(s, "")
        print(output_string[:len(output_string) - 1])
