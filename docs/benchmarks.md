# Benchmarks

The benchmarks were executed on a `MacBook Pro (16-inch, 2019)` using a `2.6 GHz 6-Core Intel Core i7` processor.

Due to the efficiency of a binary multiplier, the benchmarks had to be run using massive numbers in order to obtain any representative times.

## Unary

<table>
  <tr>
    <td>Calculation</td>
    <td><code>39 * 19 = 741</code></td>
  </tr>
  <tr>
    <td>Steps</td>
    <td><code>611'062</code></td>
  </tr>
</table>

| Name | Mean [ms] | Min [ms] | Max [ms] | Relative |
|:---|---:|---:|---:|---:|
| `unary-multiplication-using-list` | 25.9 ± 1.3 | 24.2 | 30.6 | 1.21 ± 0.26 |
| `unary-multiplication-using-map` | 24.7 ± 1.5 | 22.6 | 29.3 | 1.15 ± 0.25 |
| `unary-multiplication-using-hash-table` | 21.4 ± 4.5 | 17.6 | 51.6 | 1.00 |

<table>
  <tr>
    <td>Calculation</td>
    <td><code>612 * 288 = 176'256</code></td>
  </tr>
  <tr>
    <td>Steps</td>
    <td><code>31'276'711'910</code></td>
  </tr>
  <tr>
    <td>Time</td>
    <td><code>14min 23sec</code></td>
  </tr>
</table>

## Binary

<table>
  <tr>
    <td>Calculation</td>
    <td><code>80'473'151 * 64'955'687 = 5'227'188'808'259'737</code></td>
  </tr>
  <tr>
    <td>Steps</td>
    <td><code>61'196</code></td>
  </tr>
</table>

| Name | Mean [ms] | Min [ms] | Max [ms] | Relative |
|:---|---:|---:|---:|---:|
| `binary-multiplication-using-list` | 8.8 ± 1.6 | 6.3 | 16.6 | 1.84 ± 0.52 |
| `binary-multiplication-using-map` | 5.9 ± 0.9 | 4.8 | 10.3 | 1.24 ± 0.33 |
| `binary-multiplication-using-hash-table` | 4.8 ± 1.0 | 3.6 | 10.3 | 1.00 |
