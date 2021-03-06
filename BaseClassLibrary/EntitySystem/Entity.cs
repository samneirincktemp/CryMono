﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ComponentModel;

namespace CryEngine
{
    enum EntityMoveType
    {
        None = 0,
        Normal,
        Fly,
        Swim,
        ZeroG,

        Impulse,
        JumpInstant,
        JumpAccumulate
    }

    struct EntityMovementRequest
    {
        public EntityMoveType type;

        public Vec3 velocity;
    }

    /// <summary>
    /// </summary>
    public class Entity : StaticEntity
    {
        /// <summary>
        /// Initializes the entity, not recommended to set manually.
        /// </summary>
        /// <param name="entityId"></param>
        [EditorBrowsable(EditorBrowsableState.Never)]
        public override void InternalSpawn(uint entityId)
        {
            SpawnCommon(entityId);
            _CreateGameObjectForEntity(Id);
            OnSpawn();
        }

        public Vec3 Velocity 
        { 
            get { return _GetVelocity(Id);  }
                
            set 
            {
                EntityMovementRequest request = new EntityMovementRequest();

                request.type = EntityMoveType.Normal;
                request.velocity = value;

                _AddMovement(Id, ref request);
            }
        }
    }
}
