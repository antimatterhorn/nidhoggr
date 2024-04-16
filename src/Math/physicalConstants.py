from PYB11Generator import *

class PhysicalConstants:
    def pyinit(self, unitLm="double", unitMkg="double", unitTsec="double", unitTeK="double", unitCcou="double"):
        return

    unitLengthMeters = PYB11property("double", getter="unitLengthMeters", doc="unitLengthMeters")
    unitMassKg = PYB11property("double", getter="unitMassKg", doc="unitMassKg")
    unitTimeSec = PYB11property("double", getter="unitTimeSec", doc="unitTimeSec")
    unitTemperatureKelvin = PYB11property("double", getter="unitTemperatureKelvin", doc="unitTemperatureKelvin")
    unitChargeCoulomb = PYB11property("double", getter="unitChargeCoulomb", doc="unitChargeCoulomb")

    protonMass = PYB11property("double", getter="protonMass", doc="protonMass")
    electronMass = PYB11property("double", getter="electronMass", doc="electronMass")
    electronCharge = PYB11property("double", getter="electronCharge", doc="electronCharge")
    G = PYB11property("double", getter="G", doc="G")
    c = PYB11property("double", getter="c", doc="c")
    kB = PYB11property("double", getter="kB", doc="kB")
    molarGasConstant = PYB11property("double", getter="molarGasConstant", doc="molarGasConstant")
    kelvinsToEnergyPerMole = PYB11property("double", getter="kelvinsToEnergyPerMole", doc="kelvinsToEnergyPerMole")
    unitMassDensity = PYB11property("double", getter="unitMassDensity", doc="unitMassDensity")
    stefanBoltzmannConstant = PYB11property("double", getter="stefanBoltzmannConstant", doc="stefanBoltzmannConstant")
    blackBodyConstant = PYB11property("double", getter="blackBodyConstant", doc="blackBodyConstant")
    planckConstant = PYB11property("double", getter="planckConstant", doc="planckConstant")
    unitEnergyJ = PYB11property("double", getter="unitEnergyJ", doc="unitEnergyJ")
    epsilonZero = PYB11property("double", getter="epsilonZero", doc="epsilonZero")