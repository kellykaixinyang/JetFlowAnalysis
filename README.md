# JetFlowAnalysis
# JetTreeAnalyzer
Analysis framework for studying **jet flow** and **correlation observables** 
Includes tools to construct **Energy-Energy Correlations (EECs)** and **Two-Particle Correlations (2PCs)** for both real collision data and Pythia simulations with manually injected elliptic flow ($v_2$).

---

## 📁 Source Files

> ⚙️ **Note:** The running source file can be changed in **5 locations** inside the `Makefile`.

### **1. `EECCorr.C`**
- **Purpose:** Constructs the **1D Energy-Energy Correlation (EEC)** using the function `createEnergyDistr`.
- **Usage:** Intended for **real collision data** — this version does **not include any manual $v_2$ signal**.
- **Description:**  
  Generates EEC distributions based purely on reconstructed jets and their constituents.

---

### **2. `new_default_data_vn.C`**
- **Purpose:** Builds both **2D Two-Particle Correlations (2PC)** signal and background histograms and **1D EEC** for **Pythia-simulated pp collisions**.
- **Elliptic Flow Injection:**  
  Simulates elliptic flow ($v_2$) manually to study its effect on EEC and 2PC.

#### **Flow injection method:**
1. For each jet, generate a random number $\psi$ between $-\pi$ and $\pi$.
2. For each trigger & associated particle in that jet, set weight. For associated particles:
   $$
   \text{jet\_dau\_weight} = 1 + 2v_2\cos\big(2(\text{jet\_dau\_phi} - \psi)\big)
   $$
   where:
   - `jet_dau_phi` = azimuthal angle of daughter particle relative to jet axis (in the jet frame)
   - $v_2$ can be set between **0 and 1** in the header file `1d2d_constants.h`.

3. Similarly define:
   $$
   \text{T\_jet\_dau\_weight}
   $$
   for **trigger particles**.

#### **Weighting formulas**
- **EEC entry weight:**
  $$
  w_{\text{EEC}} = (p_T^{\text{dau}} \cdot p_T^{T}) \times (\text{T\_jet\_dau\_weight} \cdot \text{jet\_dau\_weight})
  $$
  (with $p_T$ values in the **lab frame**).

- **2PC entry weight (non-energy weighted):**
  $$
  w_{\text{2PC}} = \text{T\_jet\_dau\_weight} \cdot \text{jet\_dau\_weight}
  $$

---

### **3. `normalization.C`**
- **Purpose:** Produces **normalized 2PC yields**.
- **Description:**  
  Takes **signal** and **background histograms** as input and normalizes the 2PC signal to extract the final yield.

---

### **4. `DrawFlow.C`**
- **Purpose:** Visualization and plotting.
- **Description:**  
  Plots:
  - 1D **azimuthal projections** of the 2PC.
  - 1D **EEC distributions**.

---

## ⚙️ Configuration

### **Modify $v_2$**
Edit the constant `v2` in the header file:

```cpp
1d2d_constants.h
Acceptable range: 0 ≤ v₂ ≤ 1


Change running source file

The active source file used in compilation can be changed in 5 locations inside the Makefile.
