# encoding = utf-8
# Author: Padraic Canavan

def process_event(helper, *args, **kwargs):
    import paramiko
    import time

    cli = paramiko.client.SSHClient()
    cli.set_missing_host_key_policy(paramiko.client.AutoAddPolicy())
    cli.connect(hostname="10.0.1.X", username="pi", password="raspberry")
    stdin_, stdout_, stderr_ = cli.exec_command("python ~/script1.py")
    # time.sleep(2)    # Previously, I had to sleep for some time.
    stdout_.channel.recv_exit_status()
    lines = stdout_.readlines()
    for line in lines:
        print line

    cli.close()
    helper.log_info("Alert action turn_on_device_1 started.")

    # TODO: Implement your alert action logic here
    return 0
