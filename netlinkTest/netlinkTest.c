#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/spinlock.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/sock.h>
#include <linux/netlink.h>

#define NETLINK_TEST 22
struct sock *nl_sk = NULL;

static void
nl_data_ready(struct sk_buff *__skb)
{
        struct sk_buff *skb;
        printk(KERN_ERR"\nNl_kern: received a new service request\n");
}

int __init create_netlink_sock(void)
{
        struct netlink_kernel_cfg cfg = {
                .input = nl_data_ready,
        };

        nl_sk = netlink_kernel_create(&init_net, NETLINK_WAVE, &cfg);

        if(!nl_sk){
                return -EIO;
        }

        return 0;
}

void __exit destroy_netlink_sock(void)
{
        if(nl_sk){
                //sock_release(nl_sk->sk_socket);
        netlink_kernel_release(nl_sk);
        nl_sk = NULL;
        }
}


module_init(create_netlink_sock);
module_exit(destroy_netlink_sock);
MODULE_LICENSE("GPL");
MODULE AUTHOR("Julian");
