/*
** EPITECH PROJECT, 2026
** Project - Wrecks
** File description:
** Implementation for
** EntityContainer class
*/

#include "EntityContainer.hpp"
#include <cassert>


Wrecks::ECS::EntityContainer::EntityContainer() noexcept
    : m_Signatures()
    , m_NextID()
{

}

/**
 * Creates a new entity in
 * the container and returns it.
 * A new entity has no components
 * attached to it.
 */
auto Wrecks::ECS::EntityContainer::create() -> Entity
{
    if (!m_NextID.empty()) {
        Entity id = m_NextID.back();
        m_NextID.pop_back();
        return id;
    }

    Entity id = m_Signatures.size();
    m_Signatures.emplace_back();
    return id;
}

/**
 * Removes an existing entity
 * from the container.
 *
 * Note:
 * The given entity *must* exist
 * in the container before calling
 * remove().
 */
void Wrecks::ECS::EntityContainer::remove(Entity entity)
{
    assert(m_Signatures.size() > entity);  // GCOVR_EXCL_BR_LINE
    m_Signatures[entity].reset();
    m_NextID.push_back(entity);
}

/**
 * Sets the bit at the specified
 * bit position to 1, essentially
 * adding a component to the entity.
 */
void Wrecks::ECS::EntityContainer::addSignature(Entity entity, unsigned int bitPosition) noexcept
{
    assert(m_Signatures.size() > entity);  // GCOVR_EXCL_BR_LINE
    m_Signatures[entity][bitPosition] = 1;
}

/**
 * Sets the bit at the specified
 * bit position to 0, essentially
 * removing a component to the entity.
 */
void Wrecks::ECS::EntityContainer::removeSignature(Entity entity, unsigned int bitPosition) noexcept
{
    assert(m_Signatures.size() > entity);  // GCOVR_EXCL_BR_LINE
    m_Signatures[entity][bitPosition] = 0;
}

/**
 * Changes the signature of the
 * specified entity to the one
 * specified as parameter.
 */
void Wrecks::ECS::EntityContainer::setSignature(Entity entity, Signature newSignature) noexcept
{
    assert(m_Signatures.size() > entity);  // GCOVR_EXCL_BR_LINE
    m_Signatures[entity] = newSignature;
}

/**
 * Retrieves the signature of
 * an existing entity.
 */
auto Wrecks::ECS::EntityContainer::getSignature(Entity entity) const noexcept -> Signature
{
    assert(m_Signatures.size() > entity);  // GCOVR_EXCL_BR_LINE
    return m_Signatures[entity];
}

/**
 * Determines if the specified entity
 * contains the given signature.
 * This means the entity can have more
 * components than the specified signature.
 */
bool Wrecks::ECS::EntityContainer::hasSignature(Entity entity, Signature signature) const noexcept
{
    assert(m_Signatures.size() > entity);  // GCOVR_EXCL_BR_LINE
    return (m_Signatures[entity] & signature) == signature;
}
