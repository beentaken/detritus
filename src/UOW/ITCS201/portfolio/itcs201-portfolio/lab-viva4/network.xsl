<?xml version="1.0"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:template match="/">
		<html>
		<body>

		<table align="center" border="2">
			<tr>
				<th>Name:</th>
				<th>Arch:</th>
				<th>OS:</th>
			</tr>

			<xsl:apply-templates />
		</table>
		</body>
		</html>
	</xsl:template>

	<xsl:template match="computer">
		<tr>
			<td><xsl:apply-templates select="name" /></td>
			<td><xsl:apply-templates select="arch" /></td>
			<td><xsl:apply-templates select="os/name" /></td>
		</tr>
	</xsl:template>

</xsl:stylesheet>
