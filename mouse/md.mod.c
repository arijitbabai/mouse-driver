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
	{ 0xc4e4328b, "module_layout" },
	{ 0x9c9ad88d, "misc_deregister" },
	{ 0x7c61340c, "__release_region" },
	{ 0x50eedeb8, "printk" },
	{ 0xb1ae1708, "misc_register" },
	{ 0x1fedf0f4, "__request_region" },
	{ 0xff7559e4, "ioport_resource" },
	{ 0x2072ee9b, "request_threaded_irq" },
	{ 0xf20dabd8, "free_irq" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "435CC9DC0B0037311C840A4");
