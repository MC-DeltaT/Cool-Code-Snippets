class Extendable:
    _extension_classes: list[type] = []

    def __getattr__(self, name: str):
        for cls in self._extension_classes:
            attr = getattr(cls, name, None)
            if attr:
                if getter := getattr(attr, '__get__', None):
                    # The attribute is a property
                    return getter(self)
                else:
                    return attr(self)
        raise AttributeError(f'No attribute "{name}"')


class Foo(Extendable):
    def __init__(self, value: int) -> None:
        self._value = value

    @property
    def value(self) -> int:
        return self._value


def Extends(extend_cls: type[Extendable]):
    class ExtensionMeta(type):
        def __new__(cls, *args, **kwargs):
            new_cls = super().__new__(cls, *args, **kwargs)
            extend_cls._extension_classes.append(new_cls)
            return new_cls

    class ExtensionImpl(extend_cls, metaclass=ExtensionMeta):
        pass

    return ExtensionImpl


class FooExtension1(Extends(Foo)):
    @property
    def value2(self) -> int:
        return self.value * 2


class FooExtension2(Extends(Foo)):
    def value3(self, x: int) -> int:
        return self.value2 * x


f = Foo(42)
print(f.value)
print(f.value2)
print(f.value3(3))
