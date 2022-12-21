
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Shevchuk Ivanna <ivannas762@gmail.com>");
MODULE_DESCRIPTION("Lab7.");
MODULE_LICENSE("Dual BSD/GPL");

static uint myparam = 1;

struct my_data {
	struct list_head list;
	ktime_t ktime;
};

LIST_HEAD(my_list);

module_param(myparam, uint, 0644);
MODULE_PARM_DESC(myparam, "An unsigned integer, specifies how many times to write \"Hello, world!\"");

static int __init hello_init(void)
{

	uint i;

	if (myparam == 0 || (myparam > 5 && myparam <= 10)) {
		printk(KERN_WARNING "Warning: myparam is 0 or between 5 and 10 !\n");
	} else  {
		BUG_ON(myparam>10);
	}

	for (i = 0; i < myparam; i++) {
		struct my_data *new = 0; //kmalloc(sizeof(struct my_data *), GFP_KERNEL);

		new->ktime = ktime_get();
		list_add_tail(&new->list, &my_list);
		printk(KERN_INFO "Hello, world!\n");
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct my_data *md, *tmp;

	list_for_each_entry_safe(md, tmp, &my_list, list) {
		printk(KERN_INFO "Time: %lld\n", md->ktime);
		list_del(&md->list);
		kfree(md);
	}

}

module_init(hello_init);
module_exit(hello_exit);
