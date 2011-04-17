#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa9b6031, "module_layout" },
	{ 0xfc6256b9, "boot_tvec_bases" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x105e2727, "__tracepoint_kmalloc" },
	{ 0x4195be83, "kmem_cache_alloc" },
	{ 0xbb59c320, "kmalloc_caches" },
	{ 0x5152e605, "memcmp" },
	{ 0x37a0cba, "kfree" },
	{ 0xcb20c2bb, "neigh_for_each" },
	{ 0x5ddb0736, "arp_tbl" },
	{ 0xa58a992f, "queue_delayed_work" },
	{ 0xf9bddaa0, "__create_workqueue_key" },
	{ 0x298d1ed4, "destroy_workqueue" },
	{ 0x80bb7357, "flush_workqueue" },
	{ 0xe83fea1, "del_timer_sync" },
	{ 0xb72397d5, "printk" },
	{ 0x7129e5f8, "hex_asc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

