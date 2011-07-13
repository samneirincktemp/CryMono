﻿using System.Collections;
namespace Cemono
{
    public class BaseGame : IGame
    {
        public virtual bool Init()
        {
            return true;
        }

        public virtual bool CompleteInit()
        {
            return true;
        }

        public virtual bool Shutdown()
        {
            return true;
        }

        public virtual IEnumerator Update(bool haveFocus)
        {
        }
    }
}