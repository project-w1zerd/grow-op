# encoding = utf-8
# Author: Padraic Canavan

# This script is meant to be used as an Alert Action in Splunk.
# Use the add-on builder and copy and paste this code into the
# alert-action "Code" area. Next create a script to do whatever 
# you can think of on a raspberry pi in the /home/pi directory.
# Finally plug your pi's login detais and ip into the script 
# below as well as the path to the script. Voila

def process_event(helper, *args, **kwargs):
    import paramiko
    import time

    cli = paramiko.client.SSHClient()
    cli.set_missing_host_key_policy(paramiko.client.AutoAddPolicy())
    cli.connect(hostname="10.0.1.X", username="pi", password="raspberry")
    stdin_, stdout_, stderr_ = cli.exec_command("python ~/script1.py")
    stdout_.channel.recv_exit_status()
    lines = stdout_.readlines()
    for line in lines:
        print line

    cli.close()
    helper.log_info("Alert action turn_on_device_1 started.")

    # TODO: Implement your alert action logic here
    return 0
