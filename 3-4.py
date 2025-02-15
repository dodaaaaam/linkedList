class ListNode:

    def __init__(self, word, mean, num):
        self.word = word
        self.mean = mean
        self.num = num
        self.llink = None
        self.rlink = None


class LinkedList:

    def __init__(self):
        self.head = None

    def get_num(self, word):
        first_char = word[0].lower()
        if 'a' <= first_char <= 'z':
            return ord(first_char) - ord('a') + 1
        return 0

    def insert(self, new_node):
        if self.head is None:
            self.head = new_node
        else:
            current = self.head
            prev = None

            if new_node.num < self.head.num or (new_node.num == self.head.num
                                                and new_node.word
                                                < self.head.word):
                new_node.rlink = self.head
                self.head.llink = new_node
                self.head = new_node
                return

            while current is not None and (new_node.num > current.num or
                                           (new_node.num == current.num
                                            and new_node.word > current.word)):
                prev = current
                current = current.rlink

            if prev is None:
                new_node.rlink = self.head
                self.head.llink = new_node
                self.head = new_node
            else:
                new_node.rlink = current
                new_node.llink = prev
                if current is not None:
                    current.llink = new_node
                prev.rlink = new_node

    def print_list(self):
        current = self.head
        while current is not None:
            print(f"{current.word} : {current.mean}")
            current = current.rlink

    def search(self, input_word):
        num = self.get_num(input_word)
        current = self.head

        while current is not None:
            if current.num == num and current.word == input_word:
                return current.mean
            current = current.rlink
        return None

    def free_list(self):
        current = self.head
        while current is not None:
            temp = current
            current = current.rlink
            del temp


if __name__ == "__main__":
    linked_list = LinkedList()
    try:
        with open("randdict_utf8.TXT", "r", encoding="utf-8",
                  errors="ignore") as file:
            for line in file:
                line = line.strip()
                tokens = line.split(":")
                if len(tokens) == 2:
                    word = tokens[0].strip()
                    mean = tokens[1].strip() if tokens[1] else ""
                    num = linked_list.get_num(word)
                    new_node = ListNode(word, mean, num)
                    linked_list.insert(new_node)
    except FileNotFoundError:
        print("파일을 찾을 수 없습니다.")
        exit(1)

    linked_list.print_list()

    while True:
        input_word = input("검색할 단어를 입력하세요: ").strip()
        meaning = linked_list.search(input_word)
        if meaning:
            print(meaning)
        else:
            print("단어를 찾을 수 없습니다.")

        choice = input("계속하려면 1을 입력하세요(그만하려면 0을 입력하세요): ")
        if choice.strip() == "0":
            break

    linked_list.free_list()
