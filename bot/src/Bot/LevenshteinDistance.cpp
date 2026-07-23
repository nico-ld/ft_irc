/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LevenshteinDistance.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nile-dai <nile-dai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 15:13:43 by nile-dai          #+#    #+#             */
/*   Updated: 2026/07/23 10:33:39 by nile-dai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>

#include <iostream>

int LevenshteinDistance(std::string s1, std::string s2) {
	size_t n = s1.size();
	size_t m = s2.size();
	
	// Create matrix
	std::vector<std::vector<int> > dp(n + 1, std::vector<int> (m + 1, 0));

	// Init
	for (size_t i = 0; i < n + 1; i++)
		dp[i][0] = i;
	for (size_t i = 0; i < m + 1; i++)
		dp[0][i] = i;

	// Compute matrix
	int cost;
	for (size_t i = 1; i < n + 1; i++) {
		for (size_t j = 1; j < m + 1; j++) {
			if (s1[i - 1] == s2[j - 1])
				cost = 0;
			else
				cost = 1;
			
			dp[i][j] = std::min(
					dp[i - 1][j] + 1, 			// deletion
					std::min(dp[i][j - 1] + 1,	// insertion
					dp[i - 1][j - 1] + cost));	// substitution
		}
	}

	return (dp[n][m]);
}
