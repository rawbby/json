import re
from itertools import product
from sys import argv
from typing import Optional, Any, Union, List


class Grammar:
    _root: str
    _rules: {str, list}

    def __init__(self, path):
        with open(path) as f:
            lines = f.readlines()
        rules: {str: [str]} = dict()
        rule = lines[0][:-1]
        rules[rule] = list()
        self._root = rule
        for line in lines[1:]:
            if line[0] == '\n':
                continue
            elif line[0] == ' ':
                rules[rule].append(line[:-1])
            else:
                rule = line[:-1]
                rules[rule] = list()

        self._rules = dict()
        for key, values in rules.items():

            values_ = []
            for value in values:
                value: [str] = value.replace(':', ' : ').replace('(', ' ( ').replace(')', ' ) ').split()

                if '0' in value:
                    value.remove('0')

                value_ = []
                for v in value:
                    if re.match('^0x[0-9a-f]{2}$', v):
                        n = int(v, 16)
                        value_.append([n])
                    elif re.match('^0x[0-9a-f]{2}\\.0x[0-9a-f]{2}$', v):
                        l, u = v.split('.')
                        value_.append([n for n in range(int(l, 16), int(u, 16) + 1)])
                    else:
                        value_.append(v)

                values_.append(value_)
            self._rules[key] = values_

        for key in rules.keys():
            for alternative in self._rules[key]:
                for item in alternative:
                    if isinstance(item, list):
                        for i in range(len(item)):
                            if 31 < item[i] < 127:
                                item[i] = chr(item[i])

        # self.unify(self._root)
        print(self.expand(self._root))

    def recursive(self, rule: str):
        for alternative in self._rules[rule]:
            for item in alternative:
                if item == rule:
                    return True
        return False

    def expand(self, item: str) -> List[List[Union[str, List[int]]]]:
        if self.recursive(item):
            return [[item]]

        alternatives = []
        for alternative in self._rules[item]:
            alternatives_ = []
            for it in alternative:
                if isinstance(it, str) and re.match('^[a-z]', it):
                    if len(alternatives_):
                        alternatives_ = [list(it) for it in list(product(alternatives_, self.expand(it)))]
                    else:
                        alternatives_ = self.expand(it)
                else:
                    alternatives_.append([[it]])
            alternatives += alternatives_
            # alternatives += [list(it) for it in list(product(*alternatives_))]

        print()
        print(f"{item}:")
        for alternative in alternatives:
            print(alternative)

        return alternatives

    # noinspection PyDefaultArgument
    def unify(self, rule: str, rules: {str: list} = dict(), prefix: str = '') -> {str: list}:
        alternatives = []
        for alternative in self._rules[rule]:
            alternatives_ = []
            for item in alternative:
                if re.match('^[a-z]', item):
                    if self.recursive(item):
                        rules = {**rules, **self.unify(item, rules, f"{prefix}{rule}.")}
                        alternatives_.append([item])
                    else:
                        alternatives_.append(self._rules[item])
                else:
                    alternatives_.append([item])
            alternatives += [list(comb) for comb in list(product(*alternatives_))]
        print(alternatives)

    def empty_items(self, items: [str], rule: str) -> Optional[bool]:
        for item in items:
            if isinstance(item, list):
                return False
            if item == rule:
                return False
            if re.match('^[a-z]', item) and not self.empty(item):
                return False
        return True

    def empty(self, rule: str) -> bool:
        return any(self.empty_items(it, rule) for it in self._rules[rule])

    def first_items(self, items: [str], rule: str):
        if not len(items):
            return []

        if isinstance(items[0], list):
            return items[0]

        if re.match('^[a-z]', items[0]):
            li = []
            if items[0] != rule:
                li += self.first(items[0])
            if self.empty(items[0]):
                li += self.first_items(items[1:], rule)
            return li

        return self.first_items(items[1:], rule)

    def first(self, rule: str) -> []:
        items = {item for it in self._rules[rule] for item in self.first_items(it, rule)}
        return sorted(items)

    def last_items(self, items: [str], rule: str):
        if not len(items):
            return []

        if isinstance(items[-1], list):
            return items[-1]

        if re.match('^[a-z]', items[-1]):
            li = []
            if items[-1] != rule:
                li += self.last(items[-1])
            if self.empty(items[-1]):
                li += self.last_items(items[:-1], rule)
            return li

        return self.last_items(items[:-1], rule)

    def last(self, rule: str) -> []:
        items = {item for it in self._rules[rule] for item in self.last_items(it, rule)}
        return sorted(items)

    def next(self, rule: str, parents: [str]) -> []:
        return False

    @property
    def rules(self):
        return self._rules.keys()

    def alternatives(self, rule: str):
        return self._rules[rule]


def main():
    grammar = Grammar(argv[1])
    # for rule in grammar.rules:
    #     print(f"{rule}: {grammar.alternatives(rule)}")
    # print()
    # for rule in grammar.rules:
    #     print(f"{rule}: empty({rule})={grammar.empty(rule)}")
    # print()
    # for rule in grammar.rules:
    #     print(f"{rule}: first({rule})={grammar.first(rule)}")
    # print()
    # for rule in grammar.rules:
    #     print(f"{rule}: last({rule})={grammar.last(rule)}")


if __name__ == '__main__':
    if len(argv) == 1:
        # argv.append('json.grammar')
        argv.append('test.grammar')

    main()
