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
            var buff = new SoundBuffer("D:\\Datas\\Music\\song\\ai.mini-遇.mp3");
            var sound = new Sound2D(buff,false,true,1f);
            //sound.SetSound(buff,false,true,1f);
            Console.Read();
        }
    }
}
