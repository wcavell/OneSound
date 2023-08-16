﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XAudio
{
    [Flags]
    public enum Speaker : uint
    {
        None = 0x00000000,
        FrontLeft = 0x00000001,
        FrontRight = 0x00000002,
        FrontCenter = 0x00000004,
        LowFrequency = 0x00000008,
        BackLeft = 0x00000010,
        BackRight = 0x00000020,
        FrontLeftOfCenter = 0x00000040,
        FrontRightOfCenter = 0x00000080,
        BackCenter = 0x00000100,
        SideLeft = 0x00000200,
        SideRight = 0x00000400,
        TopCenter = 0x00000800,
        TopFrontLeft = 0x00001000,
        TopFrontCenter = 0x00002000,
        TopFrontRight = 0x00004000,
        TopBackLeft = 0x00008000,
        TopBackCenter = 0x00010000,
        TopBackRight = 0x00020000,
        Reserved = 0x7FFC0000,
        All = 0x80000000,
    }
}
