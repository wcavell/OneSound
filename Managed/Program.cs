using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OneSound;

namespace Managed
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var engine = new OneSound.SoundEngine();
            var buff = new SoundBuffer("Sound\\voice.mp3");
            var sound = new Sound2D(buff,false,true,1f); 
            sound.SetChannelMask(Speaker.FrontLeft|Speaker.FrontRight);
            var sp = sound.GetChannelMask();
            //bool hasFlag = myFlagsEnum & _flagsEnum.Option1 != 0
            sound.SetOutChannel(Speaker.None,Speaker.FrontRight);
            Console.Read();
        }
    }
}
