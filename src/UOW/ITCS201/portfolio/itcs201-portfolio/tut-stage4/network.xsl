<?xml version="1.0"?>

<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:template match="/">
		<html>
		<body>

		<ul>
		<xsl:apply-templates select="//computer">
			<li><xsl:sort select="name" datatype="string" order="ascending"/></li>
		</xsl:apply-templates>
		</ul>
		</body>
		</html>
	</xsl:template>

</xsl:stylesheet>
