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
            sound.Speaker = Speaker.FrontLeft;
            Console.Read();
            sound.Stop();
            sound.Dispose();
            buff.Dispose();
            engine.Dispose();

            Console.ReadLine();
            Console.ReadLine();
            Console.ReadLine();
        }
    }
}
