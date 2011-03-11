<?xml version="1.0"?>

<html xsl:version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<head>
		<title>GLADE Widgets for <xsl:value-of select="GTK-Interface/project/name" /></title>
	</head>
	<body>
		<h1 align="center">GLADE Widgets for <xsl:value-of select="GTK-Interface/project/name" /></h1>

		<hr size="3" />

		<table align="center" border="2">
			<tr>
					<th>Name:</th>
					<th>Class:</th>
					<th>Label:</th>
					<th>Parent Widget:</th>
			</tr>
		<xsl:for-each select="GTK-Interface//widget">
			<tr>
				<td><xsl:value-of select="name" /></td>
				<td><xsl:value-of select="class" /></td>
				<td><xsl:value-of select="label" /></td>
				<td><xsl:value-of select="parent::node()/name" /></td>
			</tr>
		</xsl:for-each>
		</table>
	</body>
</html>
