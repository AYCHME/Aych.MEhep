/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2016 Danny van Dyk
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

#include <eos/utils/observable_stub.hh>
#include <eos/utils/private_implementation_pattern-impl.hh>

namespace eos
{
    template <>
    struct Implementation<ObservableStub>
    {
        Parameters parameters;

        Kinematics kinematics;

        Options options;

        std::string name;

        UsedParameter parameter;

        Implementation(const Parameters & p, const std::string & n, ParameterUser & u) :
            parameters(p),
            name(n),
            parameter(p[n], u)
        {
        }
    };

    ObservableStub::ObservableStub(const Parameters & parameters, const std::string & name) :
        PrivateImplementationPattern<ObservableStub>(new Implementation<ObservableStub>(parameters, name, *this))
    {
    }

    ObservableStub::~ObservableStub()
    {
    }

    const std::string &
    ObservableStub::name() const
    {
        return _imp->name;
    }

    double
    ObservableStub::evaluate() const
    {
        return _imp->parameter.evaluate();
    }

    Kinematics
    ObservableStub::kinematics()
    {
        return _imp->kinematics;
    }

    Parameters
    ObservableStub::parameters()
    {
        return _imp->parameters;
    }

    Options
    ObservableStub::options()
    {
        return _imp->options;
    }

    ObservablePtr
    ObservableStub::clone() const
    {
        return ObservablePtr(new ObservableStub(_imp->parameters.clone(), _imp->name));
    }

    ObservablePtr
    ObservableStub::clone(const Parameters & parameters) const
    {
        return ObservablePtr(new ObservableStub(parameters, _imp->name));
    }
}
