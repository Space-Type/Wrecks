/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Header file for
** visitType function
*/

#pragma once

#include <stdexcept>
#include <cstddef>
#include <utility>
#include <variant>


namespace Wrecks::Utils {
    namespace Impl {
        template <std::size_t I, typename Variant>
        using VariantType = std::type_identity<std::variant_alternative_t<I, Variant>>;

        template <typename V, typename Fnc, std::size_t ...TypeIndex>
        void visitType(Fnc&& fnc, std::size_t index, std::index_sequence<TypeIndex...> indexSeq)
        {
            // The following statement may be difficult
            // to understand at first, so consider the
            // easier example of simply checking whether
            // the index is part of the index sequence:
            // ```
            // ((index == TypeIndex ? true : false) || ...)
            // ```
            // Which expands into:
            // ```
            // ((index == 0 ? true : false) || (index == 1 ? true : false) || (index == 2 ? true : false))
            // ```
            bool success =
                ((index == TypeIndex
                  ? (fnc(VariantType<TypeIndex, V>{}), true)
                  : false
                ) || ...);

            if (!success)
                throw std::out_of_range("visitType(): specified index is not a valid variant index");
        }
    }

    /**
     * visitType function.
     *
     * This function works similar to
     * std::visit(), except that it uses
     * the index of the variant's type
     * as value, and thus gives and argument
     * of type std::type_identity<T> to the
     * function (where T is the type at the
     * specified index).
     *
     * Throws an error if the given index isn't
     * a valid type in the variant.
     */
    template <typename Variant, typename Fnc>
    void visitType(Fnc&& fnc, std::size_t index)
    {
        return Impl::visitType<Variant>(std::forward<Fnc>(fnc), index, std::make_index_sequence<std::variant_size_v<Variant>>());
    }
}
