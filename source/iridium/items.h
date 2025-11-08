// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ITEMS_F019CA04_62F0_4525_996A_15F23B36F7D5
#define HEADER_ITEMS_F019CA04_62F0_4525_996A_15F23B36F7D5


#include <algorithm>
#include <list>
#include <vector>


namespace iridium {


/// \~english @brief Adds an item to a container if it is not already present.
/// \~russian @brief Добавляет элемент в контейнер, если он там еще не присутствует.
/// \~english @tparam T The type of the container, which must have `begin()`, `end()`, and `push_back()` methods, and a `value_type` typedef.
/// \~russian @tparam T Тип контейнера, который должен иметь методы `begin()`, `end()` и `push_back()`, а также typedef `value_type`.
/// \~english @param i The item to add to the container.
/// \~russian @param i Элемент для добавления в контейнер.
/// \~english @param t The container to which the item will be added.
/// \~russian @param t Контейнер, в который будет добавлен элемент.
template<typename T>
void addUnique(typename T::value_type const &i, T &t) {
    if (std::find(t.begin(), t.end(), i) == t.end())
        t.push_back(i);
}


/// \~english @brief Converts a `std::list<T>` to a `std::vector<T>`.
/// \~russian @brief Преобразует `std::list<T>` в `std::vector<T>`.
/// \~english @tparam T The type of the elements in the list and vector.
/// \~russian @tparam T Тип элементов в списке и векторе.
/// \~english @param source The source `std::list<T>` to convert.
/// \~russian @param source Исходный `std::list<T>` для преобразования.
/// \~english @return A `std::vector<T>` containing the elements from the source list.
/// \~russian @return `std::vector<T>`, содержащий элементы из исходного списка.
template<typename T>
std::vector<T> assign(std::list<T> const &source) {
    return std::vector<T> {
        std::make_move_iterator(std::begin(source)),
        std::make_move_iterator(std::end(source))
    }; // ----->
}


/// \~english @brief Checks if a source item is equal to any of the specified items.
/// \~russian @brief Проверяет, равен ли исходный элемент любому из указанных элементов.
/// \~english @tparam T The type of the items to compare.
/// \~russian @tparam T Тип сравниваемых элементов.
/// \~english @tparam TItems Variadic template parameter pack for the items to check against.
/// \~russian @tparam TItems Вариативный пакет шаблонных параметров для элементов, с которыми производится сравнение.
/// \~english @param source The item to check.
/// \~russian @param source Элемент для проверки.
/// \~english @param items_ A parameter pack of items to compare against the source item.
/// \~russian @param items_ Пакет параметров элементов для сравнения с исходным элементом.
/// \~english @return `true` if the source item matches any of the items in the pack, `false` otherwise.
/// \~russian @return `true`, если исходный элемент совпадает с любым из элементов в пакете, иначе `false`.
template<typename T, typename ... TItems>
bool checkOneOf(T const &source, TItems const &... items_) {
    std::initializer_list<T const> const &items{items_ ...};

    for (auto const &item: items)
        if (item == source)
            return true;

    return false; // ----->
}


/// \~english @brief Creates a list of shared pointers to objects of type `TObject`.
/// \~russian @brief Создает список разделяемых указателей на объекты типа `TObject`.
/// \~english @tparam TObject The type of objects to create. Must have a static `create()` method and a `TSharedPtr` typedef.
/// \~russian @tparam TObject Тип создаваемых объектов. Должен иметь статический метод `create()` и typedef `TSharedPtr`.
/// \~english @tparam TArgs Variadic template parameter pack for the arguments to `TObject::create()`.
/// \~russian @tparam TArgs Вариативный пакет шаблонных параметров для аргументов `TObject::create()`.
/// \~english @param count The number of objects to create.
/// \~russian @param count Количество создаваемых объектов.
/// \~english @param args Arguments to be forwarded to the `TObject::create()` method for each object.
/// \~russian @param args Аргументы, которые будут переданы в метод `TObject::create()` для каждого объекта.
/// \~english @return A `std::list` of `TObject::TSharedPtr` to the newly created objects.
/// \~russian @return `std::list` из `TObject::TSharedPtr` на вновь созданные объекты.
template<typename TObject, typename ... TArgs>
std::list<typename TObject::TSharedPtr> createObjects(size_t const &count, TArgs ... args) {
    std::list<typename TObject::TSharedPtr> objects;

    for (size_t i = 0; i < count; i++)
        objects.push_back(TObject::create(args ...));

    return objects; // ----->
}


/// \~english @brief Creates a list of shared pointers to an interface `TInterface`, with objects of a concrete type `TObject`.
/// \~russian @brief Создает список разделяемых указателей на интерфейс `TInterface`, с объектами конкретного типа `TObject`.
/// \~english @tparam TInterface The interface type for the shared pointers in the returned list. Must have a `TSharedPtr` typedef.
/// \~russian @tparam TInterface Тип интерфейса для разделяемых указателей в возвращаемом списке. Должен иметь typedef `TSharedPtr`.
/// \~english @tparam TObject The concrete type of objects to create. Must have a static `create()` method that returns a type convertible to `TInterface::TSharedPtr`.
/// \~russian @tparam TObject Конкретный тип создаваемых объектов. Должен иметь статический метод `create()`, возвращающий тип, совместимый с `TInterface::TSharedPtr`.
/// \~english @tparam TArgs Variadic template parameter pack for the arguments to `TObject::create()`.
/// \~russian @tparam TArgs Вариативный пакет шаблонных параметров для аргументов `TObject::create()`.
/// \~english @param count The number of objects to create.
/// \~russian @param count Количество создаваемых объектов.
/// \~english @param args Arguments to be forwarded to the `TObject::create()` method for each object.
/// \~russian @param args Аргументы, которые будут переданы в метод `TObject::create()` для каждого объекта.
/// \~english @return A `std::list` of `TInterface::TSharedPtr` to the newly created objects.
/// \~russian @return `std::list` из `TInterface::TSharedPtr` на вновь созданные объекты.
template<typename TInterface, typename TObject, typename ... TArgs>
std::list<typename TInterface::TSharedPtr> createObjects(size_t const &count, TArgs ... args) {
    std::list<typename TInterface::TSharedPtr> objects;

    for (size_t i = 0; i < count; i++)
        objects.push_back(TObject::create(args ...));

    return objects; // ----->
}


} // iridium


#endif // HEADER_ITEMS_F019CA04_62F0_4525_996A_15F23B36F7D5
