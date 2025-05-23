def TillotsonMaterial(name: str):
    name = name.lower()
    if name == "granite":
        return dict(
            rho0 = 2.7,         # g/cm³
            A = 1.80e11,        # dyn/cm²
            B = 1.80e11,
            alpha = 5.0,
            beta = 5.0,
            a = 0.5,
            b = 1.3,
            e0 = 1.67e10,       # erg/g
            eiv = 4.72e10,
            ecv = 1.82e11
        )
    elif name == "basalt":
        return dict(
            rho0 = 2.7,
            A = 2.67e11,
            B = 2.67e11,
            alpha = 5.0,
            beta = 5.0,
            a = 0.5,
            b = 1.5,
            e0 = 4.87e10,
            eiv = 4.72e10,
            ecv = 1.82e11
        )
    elif name == "iron":
        return dict(
            rho0 = 7.86,
            A = 1.28e12,
            B = 1.05e12,
            alpha = 5.0,
            beta = 5.0,
            a = 0.5,
            b = 1.5,
            e0 = 9.5e10,
            eiv = 1.0e11,
            ecv = 4.0e11
        )
    elif name == "ice":
        return dict(
            rho0 = 0.917,
            A = 9.47e9,
            B = 9.47e9,
            alpha = 10.0,
            beta = 5.0,
            a = 0.3,
            b = 0.1,
            e0 = 1.0e9,
            eiv = 7.73e9,
            ecv = 3.04e10
        )
    elif name == "aluminum":
        return dict(
            rho0 = 2.70,
            A = 7.00e10,
            B = 1.10e11,
            alpha = 5.0,
            beta = 5.0,
            a = 0.5,
            b = 1.4,
            e0 = 1.12e10,
            eiv = 1.12e10,
            ecv = 4.00e10
        )
    elif name == "water":
        return dict(
            rho0 = 1.0,
            A = 2.19e9,
            B = 9.54e8,
            alpha = 10.0,
            beta = 5.0,
            a = 0.1,
            b = 0.1,
            e0 = 3.21e9,
            eiv = 1.50e10,
            ecv = 3.50e10
        )
    else:
        raise ValueError(f"Unknown Tillotson material: {name}")
