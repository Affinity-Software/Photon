from html.parser import HTMLParser

"""PARSER FOR PHOTON"""


class Element:
    def __init__(self, tag: str, parent, self_attrs, key):
        self.tag = tag
        self.parent = parent
        self.data = None
        if parent is not None:
            self.parent_attrs = parent.self_attrs
        else:
            self.parent_attrs = None
        self.self_attrs = self_attrs
        self.next_node = None
        self.children = []
        self.key = key


class Parser(HTMLParser):
    open_tags = []
    pending_parents = []
    pending_nodes = []
    processed_nodes = []

    key = 0
    list = []

    def handle_starttag(self, tag, attrs):
        open_tags = self.open_tags

        if tag not in ['meta', 'br', 'link', 'style', 'script']:

            if open_tags == [] and tag not in ['meta', 'br']:
                open_tags.append([tag, None, attrs, None])

                # NOTE: ELEMENT CREATION AND ADDING IT TO THE LIST:
                element = Element(tag, None, attrs, self.key)
                self.key += 1

                self.place_element_in_list(element)

                open_tags.append(element)

            else:
                if tag not in ['meta', 'br']:
                    parent: Element = open_tags[len(open_tags) - 1]

                    # NOTE: ELEMENT CREATION AND ADDING IT TO THE LIST:
                    element = Element(tag, parent, attrs, self.key)
                    self.key += 1

                    self.place_element_in_list(element)

                    open_tags.append(element)

            self.open_tags = open_tags

    def handle_endtag(self, tag):
        open_tags = self.open_tags
        if tag not in ['meta', 'br']:
            del open_tags[len(open_tags) - 1]

    def handle_data(self, data):
        if any(c.isalpha() for c in data) or any(c.isnumeric() for c in data):
            open_tags = self.open_tags
            attrs = open_tags[len(open_tags) - 1].self_attrs
            parent = open_tags[len(open_tags) - 1].parent.tag
            parent_attrs = open_tags[len(open_tags) - 1].parent.self_attrs
            tag = open_tags[len(open_tags) - 1].tag

            self.place_data(data, tag, attrs, parent, parent_attrs)

    def place_element_in_list(self, element: Element):

        self.list.append([element.key, element, element.parent.key if element.parent else 0, []])
        for i in range(len(self.list) - 1, -1, -1):
            l = self.list[i]
            curr = l[1]
            if curr.self_attrs == element.parent_attrs and curr.tag == element.parent.tag:
                self.list[i][3].append(element.key)
                break

    def place_data(self, data, handle_tag, handle_attrs, handle_parent_tag, handle_parent_attrs):
        for index in range(len(self.list) - 1, -1, -1):
            element = self.list[index]
            node = element[1]
            if node.data != None:
                continue

            if node.tag == handle_tag and node.self_attrs == handle_attrs and node.parent.tag == handle_parent_tag and node.parent.self_attrs == handle_parent_attrs:
                self.list[index][1].data = data


def parse(path):
    html_file = open(path)
    html_content = html_file.read()

    parser = Parser()
    parser.feed(html_content)
    return parser.list
