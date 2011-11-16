/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2010, 2011 Danny van Dyk
 *
 * This file is part of the EOS project. EOS is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License version 2, as published by the Free Software Foundation.
 *
 * EOS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <eos/rare-b-decays/inclusive-b-to-s-gamma.hh>
#include <eos/utils/integrate.hh>
#include <eos/utils/kinematic.hh>
#include <eos/utils/log.hh>
#include <eos/utils/model.hh>
#include <eos/utils/private_implementation_pattern-impl.hh>
#include <eos/utils/qcd.hh>

namespace eos
{
    /* Minimal */

    template <>
    struct Implementation<BToXsGamma<Minimal>>
    {
        std::shared_ptr<Model> model;

        UsedParameter m_b_MSbar;

        UsedParameter alpha_e;

        UsedParameter br_bcsl;

        UsedParameter uncertainty;

        Implementation(const Parameters & p, const Options & o, ParameterUser & u) :
            model(Model::make(o.get("model", "SM"), p, o)),
            m_b_MSbar(p["mass::b(MSbar)"], u),
            alpha_e(p["QED::alpha_e(m_b)"], u),
            br_bcsl(p["exp::BR(B->X_clnu)"], u),
            uncertainty(p["B->X_sgamma::uncertainty"], u)
        {
            if ("SM" != o.get("model", "SM"))
            {
                Log::instance()->message("B->X_sgamma.model", ll_error)
                    << "B->X_sgamma is not yet capable to handle models beyond SM, e.g. for helicity flipped operators; use it carefully";
            }

            u.uses(*model);
        }

        double m_c_pole() const
        {
            return 1.6;
        }

        double branching_ratio() const
        {
            static const double sm = 3.15e-4;
            static const double sm_delta = 0.23e-4;
            static const double c7sm = -0.3;

            double m_c_hat = m_c_pole() / model->m_b_pole();
            double z = pow(m_c_hat, 2);
            double z2 = pow(z, 2), z3 = z * z2, z4 = z3 * z, lnz = log(z);

            // cf. [BMU1999], Eq. (46), p. 16
            double g = 1.0 - 8.0 * z + 8.0 * z3 - z4 - 12.0 * z2 * lnz;
            double kappa = 1.0 - 2.0/3.0 * model->alpha_s(model->m_b_pole()) / M_PI * (1.5 + (M_PI * M_PI - 31.0 / 4.0) * pow(1.0 - m_c_hat, 2));

            double ckm = norm(model->ckm_tb() * conj(model->ckm_ts()) / model->ckm_cb());
            WilsonCoefficients<BToS> wc = model->wilson_coefficients_b_to_s();
            complex<double> c7np = wc.c7() - c7sm;

            double result = (sm + sm_delta * uncertainty)
                + 6.0 * alpha_e / M_PI * br_bcsl * ckm / g / kappa * (norm(c7np) + 2.0 * real(c7np * c7sm));

            // Make sure the approximate BR is positive definite.
            return std::max(result, 0.0);
        }
    };

    BToXsGamma<Minimal>::BToXsGamma(const Parameters & parameters, const Options & options) :
        PrivateImplementationPattern<BToXsGamma<Minimal>>(new Implementation<BToXsGamma<Minimal>>(parameters, options, *this))
    {
    }

    BToXsGamma<Minimal>::~BToXsGamma()
    {
    }

    double
    BToXsGamma<Minimal>::integrated_branching_ratio() const
    {
        return _imp->branching_ratio();
    }
}