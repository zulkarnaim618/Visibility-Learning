# Visibility-Learning
CSE 400 Thesis

## Problem Formulation

Given a scene $\mathcal{S}$ consisting of obstacles $O = \{O_1, O_2, \dots, O_n\}$ and two query points $p, q \in \mathbb{R}^d$, the visibility function $V(p, q)$ determines whether there exists an unobstructed line-of-sight between $p$ and $q$. This can be formally defined as:

\[
V(p, q) =
\begin{cases} 
1, & \text{if } \forall x \in \overline{pq}, x \notin O, \\
0, & \text{otherwise}.
\end{cases}
\]

where $\overline{pq}$ represents the line segment connecting $p$ and $q$. The challenge is to compute $V(p, q)$ efficiently in complex scenes containing numerous obstacles.

