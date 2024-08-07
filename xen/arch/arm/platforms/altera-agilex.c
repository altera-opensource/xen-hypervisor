/*
 * xen/arch/arm/platforms/altera-agilex.c
 *
 * Altera Agilex platform setup
 *
 * Rabara Niravkumar L <niravkumar.l.rabara@intel.com>
 * Copyright (c) 2024 Intel Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <asm/regs.h>
#include <xen/sched.h>
#include <asm/platform.h>
#include <asm/smccc.h>

static const char * const agilex_dt_compat[] __initconst =
{
    "intel,socfpga-agilex",
    "intel,socfpga-agilex5",
    NULL
};

static bool agilex_smc(struct cpu_user_regs *regs)
{
    struct arm_smccc_res res;

    /*If SMCCC 1.1 is not available something is wrong, don't try to handle it.*/
    if ( !cpus_have_const_cap(ARM_SMCCC_1_1) )
    {
        printk_once(XENLOG_WARNING
                    "Agilex Error: no SMCCC 1.1 support. Disabling firmware calls\n");

        return false;
    }

    arm_smccc_1_1_smc(get_user_reg(regs, 0),
                      get_user_reg(regs, 1),
                      get_user_reg(regs, 2),
                      get_user_reg(regs, 3),
                      get_user_reg(regs, 4),
                      get_user_reg(regs, 5),
                      get_user_reg(regs, 6),
                      get_user_reg(regs, 7),
                      &res);

    set_user_reg(regs, 0, res.a0);
    set_user_reg(regs, 1, res.a1);
    set_user_reg(regs, 2, res.a2);
    set_user_reg(regs, 3, res.a3);

    return true;
}

PLATFORM_START(Altera_agilex, "Altera Agilex")
    .compatible = agilex_dt_compat,
    .smc = agilex_smc,
PLATFORM_END

/*
 * Local variables:
 * mode: C
 * c-file-style: "BSD"
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
