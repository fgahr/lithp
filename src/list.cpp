#include <lithp/lithp.hpp>

namespace lithp {
using namespace runtime;

List::List(Object *__car, Object *__cdr) : _car{__car}, _cdr{__cdr} {}

void List::repr(std::ostream &out) {
    out << "(";
    _car->repr(out);
    Object *rest = _cdr;
    while (List::is_instance(rest)) {
        out << " ";
        List *as_list = List::cast(rest);
        as_list->_car->repr(out);
        rest = as_list->_cdr;
    }

    if (is_null(rest)) {
        out << ")";
    } else {
        out << " . ";
        rest->repr(out);
        out << ")";
    }
}

RefStream List::refs() {
    // NOTE: Nested references need to be adjusted first in a gc run
    return RefStream::concat(_car->refs(), _cdr->refs(),
                             RefStream::of(&_car, &_cdr));
}
Object *List::copy_to(void *mem) {
    return new (mem) List{_car, _cdr};
}

bool List::empty() {
    return is_null(_car) && is_null(_cdr);
}
void List::set_car(Object *obj) {
    _car = obj;
}
void List::set_cdr(Object *obj) {
    _cdr = obj;
}
Object *List::car() {
    return _car;
}
Object *List::cdr() {
    return _cdr;
}

bool List::is_instance(Object *obj) {
    LITHP_CHECK_TYPE(obj, List);
}
List *List::cast(Object *obj) {
    if (is_null(obj)) {
        return nullptr;
    }

    if ((obj)->type() != Type::List) {
        throw std::logic_error{"illegal type conversion from " +
                               type_name((obj)->type()) + " to " +
                               type_name(Type::List)};
    }
    return static_cast<List *>(obj);
}

List *List::make(Object *car, Object *cdr) {
    return HEAP_NEW(List){car, cdr};
}

List *List::of(size_t n, Object **objects) {
    if (n == 0) {
        return nullptr;
    }

    stack::new_frame(nil());
    List *head = List::make(objects[0], nil());
    stack::Ref href = stack::push(head);
    List *current = List::cast(stack::get(href));
    stack::Ref cref = stack::push(current);
    stack::Ref nref = stack::push(nil());

    for (size_t i = 1; i < n; i++) {
        stack::set(nref, List::make(objects[i], nil()));
        current = List::cast(stack::get(cref));
        current->set_cdr(stack::get(nref));
        stack::set(cref, current->cdr());
    }

    List *list = List::cast(stack::get(href));
    stack::yield_frame();
    return list;
}

List *List::of(std::vector<Object *> objects) {
    if (objects.empty()) {
        return nullptr;
    }

    stack::new_frame(nil());
    size_t nargs = objects.size();
    Object **args = stack::ptr();

    for (Object *obj : objects) {
        stack::push(obj);
    }

    List *list = List::of(nargs, args);
    stack::yield_frame();
    return list;
}

size_t List::length(List *list) {
    if (is_null(list)) {
        return 0;
    }

    size_t len = 0;
    for (Object *rest = list; !is_null(rest); ++len) {
        if (!List::is_instance(rest)) {
            throw std::runtime_error{"not a list: " + to_string(rest)};
        }
        rest = List::cast(rest)->cdr();
    }
    return len;
}
} // namespace lithp
