#ifndef ENTITYXINCLUDE_HPP
#define ENTITYXINCLUDE_HPP

#include <entityx/entityx.h>
#include <entityx/Entity.h>

typedef entityx::EntityX EntityX;

typedef entityx::Entity Entity;
typedef uint64_t entityxId;

using entityx::ComponentHandle;
using entityx::System;
typedef entityx::EntityManager EntityManager;
typedef entityx::EventManager EventManager;
typedef entityx::TimeDelta TimeDelta;

#endif //ENTITYXINCLUDE_HPP
