# README Documentation

## Home Server Setup Guide

This documentation covers the steps to install and set up Ubuntu Server Edition on your machine and configure various services.

---

## Installing Ubuntu on Your Machine

To begin, we need to install Ubuntu Server Edition. Follow these steps:

1. **Download Ubuntu Server Edition**  
    Download the Ubuntu Server image from the official website:
    
    - [Download Ubuntu Server Edition](https://ubuntu.com/download/server#downloads)
2. **Create a Bootable USB Drive**  
    For Windows, use [RUFUS](https://rufus.ie/). For Linux, use the [Popsicle USB Flasher](https://github.com/pop-os/popsicle). Flash the downloaded disk image onto a USB flash drive.
    
3. **Boot from USB**  
    Insert the USB into your machine and boot into it by hitting the designated key during the boot-up process (this varies by manufacturer).
    
4. **Run the Ubuntu Server Setup**  
    The setup process is straightforward:
    
    - Select your language.
    - Choose ‘**Continue without updating**’ (you will update the system later).
    - Set up your keyboard layout (select ‘English (US)’ if applicable).
    - Choose your network adapter. For a server setup, it’s recommended to use a wired Ethernet connection.
    - Leave the proxy address blank.
    - Keep the Ubuntu mirror at default.
    - Choose the disk you want to install Ubuntu on. Select the entire disk and avoid the LVM group option.
    - Set your username, server name, and password.
    - Select ‘Install OpenSSH server’ when prompted.
    - Skip Snap installs for now.
5. **Finalize Installation**  
    Once the installation completes, remove the USB drive and reboot your server. After reboot, log in using the username and password you set up during installation.
    
6. **Update System**  
    Run the following commands to update your system:
    

1. `sudo apt update && sudo apt upgrade`
    

---
### **1. SSH Connection Issue**

- **Problem:** During the first SSH connection attempt, outdated syntax in the connection string caused confusion. After being prompted for a password, an error log appeared.
- **Error Log:**

`ssh: Could not resolve hostname user@192.168.X.X: Name or service not known`

- **Cause:** Incorrect syntax in the SSH command, specifically the hostname was misformatted.
- **Solution:** Used the correct syntax:

`ssh user@192.168.X.X`

Double-checked hostname, username, and IP address.

---

### **2. SSH Authentication Failure**

- **Problem:** Despite using the correct SSH syntax, authentication repeatedly failed even with the right password.
- **Error Log:**

`authentication failure; logname= uid=0 euid=0 tty=ssh ruser= rhost=192.168.1.181 user=erdilli Failed password for erdilli from 192.168.1.181 port 55846 ssh2`

- **Cause:** SSH keys or password authentication settings were misconfigured on the server.
- **Solution:**
    - Ensured password authentication was enabled in the SSH configuration file (`/etc/ssh/sshd_config`):

`PasswordAuthentication yes`

- Restarted the SSH service:

`sudo systemctl restart sshd`

- Verified user permissions and reset the password if necessary.

---

### **3. Cron Job Misconfiguration**

- **Problem:** Automated Python scripts failed to run via cron jobs.
- **Cause:** The environment variables in the cron job were not properly configured, leading to missing dependencies.
- **Solution:** Specified the full paths to Python and script dependencies in the cron job entry:

`* * * * * /usr/bin/python3 /path/to/script.py`

---

### **4. Docker Container Restart Loop**

- **Problem:** One of the Docker containers was stuck in a restart loop.
- **Cause:** The container lacked proper volume mounts, and a misconfigured `docker-compose.yml` file caused persistent failures.
- **Solution:** Corrected volume mounts in the `docker-compose.yml`:

`volumes:   - ./data:/app/data`

- Restarted the Docker container:

`docker-compose down && docker-compose up -d`

---

### **5. Resource Constraints on Hardware**

- **Problem:** Limited RAM and CPU resources on the Lenovo and Samsung hardware led to performance issues, especially when running multiple services.
- **Cause:** Overcommitment of resources by Docker containers and unnecessary background processes.
- **Solution:** Optimized Docker container resource allocation:

`deploy:   resources:     limits:       memory: 512M       cpus: '0.5'`

- Disabled unnecessary services and optimized container images.

---

### **6. File Permission Issues**

- **Problem:** Scripts failed to execute due to permission errors.
- **Cause:** Incorrect file permissions for script files.
- **Solution:** Set the correct permissions for executable scripts:

`chmod +x script.py`

- Ensured proper ownership:

`chown user:user script.py`

---

### **7. Network Bottleneck and Firewall Configuration**

- **Problem:** Network latency and bottlenecks occurred while accessing remote services.
- **Cause:** Misconfigured firewall rules and network throttling on older hardware.
- **Solution:**
    - Adjusted firewall rules to allow necessary traffic.
    - Monitored traffic using tools like `htop` and `iftop`.
    - Prioritized essential services in network QoS settings.

---

### **8. LLM Web UI Access Issue**

- **Problem:** The lightweight LLMs failed to start the web UI on the designated port.
- **Cause:** Port conflicts with other services running on the same hardware.
- **Solution:** Changed the default port for the LLM web UI:

`--port 8081`

- Ensured the port was open in the firewall:

`ufw allow 8081`

---

### **9. Nextcloud Performance Issues**

- **Problem:** Nextcloud was slow and occasionally unresponsive.
- **Cause:** Insufficient memory allocation and high disk I/O.
- **Solution:**
    - Enabled caching via Redis.
    - Adjusted PHP memory limits in `php.ini`:

`memory_limit = 512M`

- Scheduled periodic database optimization tasks.

---

### **10. Nextcloud Installation Issue**

- **Problem:** Upon checking system logs using:

`sudo tail -f /var/log/syslog`

It indicated that Nextcloud was not fully installed, and only limited functionality was available.

- **Cause:** Incomplete or failed Nextcloud installation/configuration.
- **Solution:**
    - Re-ran the Nextcloud setup:

`sudo nextcloud.occ maintenance:install`

- Verified database and storage configurations.
- Ensured required dependencies were installed:

`sudo apt install php-gd php-json php-curl php-mbstring php-intl php-imagick php-xml php-zip`

- Restarted Apache/Nginx:

`sudo systemctl restart apache2`

---

### **11. Passwordless Boot Configuration**

- **Problem:** The server required a password on reboot, causing delays in automated processes.
- **Cause:** Encrypted or user-password-protected boot process.
- **Solution:**
    - Edited the `/etc/sudoers` file to allow passwordless operations for reboot:

`user ALL=(ALL) NOPASSWD: /sbin/reboot`

- Disabled password prompt on startup via `systemctl`:

`sudo systemctl enable autologin@<your_username>`

- Ensured `autologin` was properly configured in `/etc/systemd/system/getty@tty1.service.d/autologin.conf`.

---

### **12. Preventing Sleep and Lid Actions**

- **Problem:** The server would sleep or suspend when the lid was closed or after a period of inactivity.
- **Cause:** Default power management settings.
- **Solution:**
    - Edited `/etc/systemd/logind.conf` and added the following lines:

`HandleSuspendKey=ignore HandleLidSwitch=ignore HandleLidSwitchDocked=ignore`

- Restarted the `systemd-logind` service:

`sudo systemctl restart systemd-logind`

- Verified that the changes were applied:

`systemctl status systemd-logind`

---

### **13. Setting a Static IP Address**

- **Problem:** The server's IP address changes dynamically after every reboot or network reset, causing disruptions when accessing it remotely via SSH or services like Nextcloud.
- **Solution:**

To set a static IP address on a Linux server using Netplan:

1. **Edit Netplan Configuration:**  
    Open the Netplan configuration file:

`sudo nano /etc/netplan/00-installer-config.yaml`

2. **Configure Static IP:**  
    Replace or adjust the existing configuration to something like this:

`network:   version: 2   ethernets:     enp4s0:       dhcp4: no       addresses:         - 192.168.1.100/24  # Static IP Address       gateway4: 192.168.1.1  # Default Gateway       nameservers:         addresses:           - 8.8.8.8           - 8.8.4.4`

- **Apply Changes:**

`sudo netplan apply`

- **Verify Configuration:**

`ip a`

---

### **14. Wake-on-LAN (WoL) Configuration**

- **Problem:** Some devices have Wake-on-LAN enabled (`enp4s0` and `wlp3s0`), while others (USB devices) are disabled. The server might not wake up reliably.
- **Solution:** Enable Wake-on-LAN permanently.

1. **Install ethtool (if not already installed):**

`sudo apt install ethtool`

2. **Enable Wake-on-LAN:**

`sudo ethtool -s enp4s0 wol g`

3. **Make it Persistent Across Reboots:**  
    Create or edit a systemd service:

`sudo nano /etc/systemd/system/wol@.service`

Add the following content:

`[Unit] Description=Wake-on-LAN for %i [Service] Type=oneshot ExecStart=/sbin/ethtool -s %i wol g [Install] WantedBy=default.target`

4. **Enable the Service:**

`sudo systemctl enable wol@enp4s0.service sudo systemctl start wol@enp4s0.service`

5. **Verify WoL Status:**

`sudo ethtool enp4s0 | grep Wake-on`

---

### **15. Testing Wake-on-LAN**

To test WoL from another computer:

1. Install `wakeonlan` tool:

`sudo apt install wakeonlan`

2. Send a Wake-on-LAN packet using the server's MAC address:

`wakeonlan <Server_MAC_Address>`

Replace `<Server_MAC_Address>` with the MAC address of `enp4s0` (`ip a` will display it).
