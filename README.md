# reinforcement-learning
This repository is meant to guide my study in reinforcement learning and related topics. I'll be using the book Reinforcement Learning: An Introduction 2nd Edition initially. Any other resources used will be cited.


|Chapter                                       |Situation   |Date Finished |
|----------------------------------------------|------------|--------------|
|1. Introduction                               |Finished    |20/05/2025    |
|2. Multi-armed Bandits                        |Finished    |27/05/2025    |
|3. Finite MDPs                                |Finished    |29/05/2025    |
|4. Dynamic Programming                        |Finished    |09/06/2025    |
|5. Monte Carlo Methods                        |Finished    |04/07/2025    |
|6. TD Learning                                |Finished    |22/07/2025    |
|7. n-steps Bootstrapping                      |Finished    |09/08/2025    |
|8. Planning and Learning with Tabular Methods |Ongoing     |              |
|9. On-Policy Prediction with Approximation    |Not started |              |
|10. On-Policy Control with Approximation      |Not started |              |
|11. Off-Policy Methods with Approximation     |Not started |              |
|12. Eligibility Traces                        |Not started |              |
|13. Policy Gradient Methods                   |Not started |              |
|14. Psychology                                |Not started |              |
|15. Neuroscience                              |Not started |              |
|16. Applications                              |Not started |              |
|17. Frontiers                                 |Not started |              |

The book can be found on the writers repository: http://incompleteideas.net/book/


# Emerging Subfields in Reinforcement Learning (ChatGPT)
## 1. Offline Reinforcement Learning

**Goal:** Train agents from *pre-collected data* without further environment interaction.

**Why it's important:**
- Critical for domains like robotics, healthcare, and recommendation systems
- Avoids unsafe or expensive online exploration

**Key Methods:**
- CQL (Conservative Q-Learning)
- BCQ (Batch-Constrained Q-Learning)
- AWAC (Advantage-Weighted Actor-Critic)
- IQL (Implicit Q-Learning)

---

## 2. Model-Based Reinforcement Learning (MBRL)

**Goal:** Learn a model of the environment and use it to plan or accelerate learning.

**Why it's important:**
- Dramatically improves sample efficiency
- Crucial for real-world robotics and constrained domains

**Key Methods:**
- PETS (Probabilistic Ensembles)
- MBPO (Model-Based Policy Optimization)
- Dreamer / DreamerV3

---

## 3. Hierarchical RL / Skill Learning

**Goal:** Decompose long tasks into reusable skills or subpolicies.

**Why it's important:**
- Enables long-horizon reasoning
- Crucial in robotics and planning

**Key Methods:**
- Option-Critic
- Feudal Networks
- DIAYN (Diversity is All You Need)

---

## 4. Sim2Real Transfer

**Goal:** Train agents in simulation and successfully deploy them in the real world.

**Why it's important:**
- Essential in robotics (safe, fast, cheap training)
- Needed to bridge the sim-real gap

**Key Techniques:**
- Domain randomization
- Domain adaptation
- Sim2Real fine-tuning

---

## 5. Imitation Learning & Inverse RL

**Goal:** Learn from expert demonstrations instead of relying on reward functions.

**Why it's important:**
- Defining rewards is hard in real-world tasks
- Easy to collect human demos

**Key Methods:**
- Behavior Cloning (BC)
- DAgger (Dataset Aggregation)
- GAIL (Generative Adversarial Imitation Learning)
- Maximum Entropy IRL

---

## 6. Exploration & Intrinsic Motivation

**Goal:** Motivate agents to explore in the absence of dense rewards.

**Why it's important:**
- Sparse-reward problems remain difficult
- Leads to better generalization and curiosity

**Key Ideas:**
- Random Network Distillation (RND)
- Count-based exploration
- Intrinsic curiosity modules (ICM)

---

## 7. RL + Foundation Models (LLMs, Vision, etc.)

**Goal:** Combine RL with pretrained language or vision models for generality and grounding.

**Why it's important:**
- Foundation of modern AI systems like ChatGPT (RLHF)
- Used in robotics with language goals or multimodal input

**Key Trends:**
- RLHF (Reinforcement Learning with Human Feedback)
- LLMs as planners (e.g., Code as Policies, SayCan)
- Language-conditioned control

---

## 8. Self-Supervised & Unsupervised RL

**Goal:** Pretrain representations or policies without extrinsic rewards.

**Why it's important:**
- Reduces label/reward dependency
- Boosts transfer and generalization

**Key Work:**
- CURL, DrQ (visual RL)
- DIAYN, APS, SMM
- Dreamer with unsupervised objectives

---

## 9. Lifelong / Continual RL

**Goal:** Learn multiple tasks over time without forgetting past ones.

**Why it's important:**
- Enables lifelong learning in real robots
- Promotes scalability and memory efficiency

**Key Concepts:**
- Catastrophic forgetting
- Meta-learning for task adaptation
- Progressive networks, curriculum learning

---

## 10. Safe RL and Risk-Aware Learning

**Goal:** Ensure that RL agents respect safety constraints and avoid catastrophic actions.

**Why it's important:**
- Vital in robotics, healthcare, and autonomous systems
- Safety-critical environments demand guarantees

**Key Approaches:**
- Constrained MDPs
- Shielded RL
- Distributional RL (C51, QR-DQN)
- Lyapunov-based control

---

## 11. Multi-Agent Reinforcement Learning (MARL)
**Goal:** Train multiple agents that interact, cooperate, or compete within a shared environment.

**Why it's important:**
- Many real-world problems (robot teams, autonomous vehicles, games, smart grids) involve multiple agents.
- Addresses challenges like non-stationarity, credit assignment, and coordination.

**Key Methods:**
-Centralized Training with Decentralized Execution (CTDE)
-Value Decomposition Networks (VDN), QMIX
-Policy gradients for multi-agent (MADDPG)
-Mean Field RL and opponent modeling

---
